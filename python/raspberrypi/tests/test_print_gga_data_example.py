import ast
import unittest
from pathlib import Path

EXAMPLE = Path(__file__).resolve().parents[1] / "examples" / "print_gga_data.py"


def functions(tree):
    return {
        node.name: node
        for node in tree.body
        if isinstance(node, ast.FunctionDef)
    }


def attr_calls(node):
    found = []
    if isinstance(node, ast.Call) and isinstance(node.func, ast.Attribute):
        found.append(node.func.attr)
    for child in ast.iter_child_nodes(node):
        found.extend(attr_calls(child))
    return found


def print_first_args(node):
    args = []
    if (
        isinstance(node, ast.Call)
        and isinstance(node.func, ast.Name)
        and node.func.id == "print"
        and node.args
        and isinstance(node.args[0], ast.Constant)
    ):
        args.append(node.args[0].value)
    for child in ast.iter_child_nodes(node):
        args.extend(print_first_args(child))
    return args


def main_guard(tree):
    for node in tree.body:
        if (
            isinstance(node, ast.If)
            and isinstance(node.test, ast.Compare)
            and isinstance(node.test.left, ast.Name)
            and node.test.left.id == "__name__"
        ):
            return node
    return None


class PrintGgaDataExampleTest(unittest.TestCase):
    def setUp(self):
        self.tree = ast.parse(EXAMPLE.read_text(encoding="utf-8"))
        self.fns = functions(self.tree)

    def test_setup_and_loop_replace_main(self):
        self.assertIn("setup", self.fns)
        self.assertIn("loop", self.fns)
        self.assertNotIn("main", self.fns)

    def test_module_level_rtk_assigned_in_setup(self):
        self.assertIn("setup", self.fns)
        self.assertIn("loop", self.fns)
        self.assertNotIn("main", self.fns)
        assigned_none = False
        for node in self.tree.body:
            if isinstance(node, ast.Assign):
                for target in node.targets:
                    if isinstance(target, ast.Name) and target.id == "rtk":
                        assigned_none = (
                            isinstance(node.value, ast.Constant)
                            and node.value.value is None
                        )
        self.assertTrue(assigned_none)
        setup_globals = [
            node
            for node in ast.walk(self.fns["setup"])
            if isinstance(node, ast.Global) and "rtk" in node.names
        ]
        self.assertTrue(setup_globals)
        self.assertIn("begin", attr_calls(self.fns["setup"]))
        self.assertEqual(
            print_first_args(self.fns["setup"]),
            ["RTK init error!", "RTK init success."],
        )

    def test_loop_prints_gga_and_returns_on_failure(self):
        self.assertIn("setup", self.fns)
        self.assertIn("loop", self.fns)
        self.assertNotIn("main", self.fns)
        loop = self.fns["loop"]
        self.assertEqual(
            attr_calls(loop),
            [
                "update_gga",
                "sleep",
                "get_time",
                "get_latitude",
                "get_latitude_direction",
                "get_longitude",
                "get_longitude_direction",
                "get_satellites",
                "get_altitude",
                "get_geoid_separation",
                "get_fix_quality",
                "get_diff_age",
                "sleep",
            ],
        )
        self.assertEqual(
            print_first_args(loop),
            [
                "GGA update failed.",
                "time:",
                "latitude:",
                "latitudeDir:",
                "longitude:",
                "longitudeDir:",
                "satellites:",
                "altitude:",
                "geoid separation:",
                "fix quality:",
                "diff age:",
                "--------------------------------",
            ],
        )
        self.assertTrue(any(isinstance(node, ast.Return) for node in ast.walk(loop)))
        self.assertFalse(any(isinstance(node, ast.Continue) for node in ast.walk(loop)))
        sleeps = [
            node.args[0].value
            for node in ast.walk(loop)
            if isinstance(node, ast.Call)
            and isinstance(node.func, ast.Attribute)
            and node.func.attr == "sleep"
        ]
        self.assertEqual(sleeps, [0.5, 0.5])

    def test_main_guard_calls_setup_then_loop(self):
        guard = main_guard(self.tree)
        self.assertIsNotNone(guard)
        self.assertEqual(len(guard.body), 2)
        setup_call = guard.body[0]
        self.assertIsInstance(setup_call, ast.Expr)
        self.assertEqual(setup_call.value.func.id, "setup")
        loop_while = guard.body[1]
        self.assertIsInstance(loop_while, ast.While)
        self.assertEqual(loop_while.body[0].value.func.id, "loop")


if __name__ == "__main__":
    unittest.main()
