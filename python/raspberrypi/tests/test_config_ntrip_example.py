import ast
import unittest
from pathlib import Path

EXAMPLE = Path(__file__).resolve().parents[1] / "examples" / "config_ntrip.py"


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


def keyword_names(node, call_name):
    if (
        isinstance(node, ast.Call)
        and isinstance(node.func, ast.Attribute)
        and node.func.attr == call_name
    ):
        return [keyword.arg for keyword in node.keywords]
    for child in ast.iter_child_nodes(node):
        found = keyword_names(child, call_name)
        if found:
            return found
    return []


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


class ConfigNtripExampleTest(unittest.TestCase):
    def setUp(self):
        self.tree = ast.parse(EXAMPLE.read_text(encoding="utf-8"))
        self.fns = functions(self.tree)

    def test_setup_and_loop_replace_main(self):
        self.assertIn("setup", self.fns)
        self.assertIn("loop", self.fns)
        self.assertNotIn("main", self.fns)

    def test_setup_retries_begin_and_config_ntrip(self):
        self.assertIn("setup", self.fns)
        self.assertIn("loop", self.fns)
        self.assertNotIn("main", self.fns)
        setup = self.fns["setup"]
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
        self.assertTrue(any(
            isinstance(node, ast.Global) and "rtk" in node.names
            for node in ast.walk(setup)
        ))
        self.assertEqual(attr_calls(setup), ["begin", "sleep", "config_ntrip", "sleep"])
        self.assertEqual(
            keyword_names(setup, "config_ntrip"),
            ["host", "port", "user", "password", "mountpoint"],
        )
        self.assertEqual(
            print_first_args(setup),
            [
                "RTK init error!",
                "RTK init success.",
                "NTRIP config error!",
                "NTRIP config success.",
            ],
        )

    def test_loop_only_sleeps_one_second(self):
        self.assertIn("setup", self.fns)
        self.assertIn("loop", self.fns)
        self.assertNotIn("main", self.fns)
        loop = self.fns["loop"]
        self.assertEqual(attr_calls(loop), ["sleep"])
        sleeps = [
            node.args[0].value
            for node in ast.walk(loop)
            if isinstance(node, ast.Call)
            and isinstance(node.func, ast.Attribute)
            and node.func.attr == "sleep"
        ]
        self.assertEqual(sleeps, [1])

    def test_main_guard_calls_setup_then_loop(self):
        guard = main_guard(self.tree)
        self.assertIsNotNone(guard)
        self.assertEqual(len(guard.body), 2)
        self.assertEqual(guard.body[0].value.func.id, "setup")
        self.assertIsInstance(guard.body[1], ast.While)
        self.assertEqual(guard.body[1].body[0].value.func.id, "loop")


if __name__ == "__main__":
    unittest.main()
