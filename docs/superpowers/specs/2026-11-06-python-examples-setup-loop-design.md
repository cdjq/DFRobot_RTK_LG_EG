# Python examples: setup() and loop()

## Goal

Restructure the Raspberry Pi examples so each one reads like the matching Arduino sketch: one-time work in `setup()`, repeated work in `loop()`.

## Scope

Change only these files:

- `python/raspberrypi/examples/print_gga_data.py`
- `python/raspberrypi/examples/config_ntrip.py`
- `python/raspberrypi/examples/enable_sd_csv.py`

Leave the library, READMEs, and Arduino examples unchanged. Keep serial constants, retry loops, print text, comments, and sleep intervals as they are.

## Structure

Each example uses the same skeleton. `rtk` is a module-level name so `loop()` can use the object created in `setup()`, matching the global `rtk` in the `.ino` files.

```python
rtk = None

def setup():
    global rtk
    rtk = DFRobot_RTK_LG_EG(port=SERIAL_PORT, baud=BAUD)
    # begin(), and any one-shot configuration

def loop():
    # repeated work, or a single sleep when there is none

if __name__ == '__main__':
    setup()
    while True:
        loop()
```

`main()` is removed. The `while True` that used to live inside `main()` moves to the `__main__` block and calls `loop()` once per iteration.

## Per example

**`print_gga_data.py`**

- `setup()` creates `rtk`, retries `begin()` until it returns 0, then prints success.
- `loop()` calls `update_gga()`. On failure it prints the failure line and sleeps 0.5 seconds. On success it prints the same GGA fields as today, then sleeps 0.5 seconds.

**`config_ntrip.py`**

- `setup()` retries `begin()`, then retries `config_ntrip()` with the existing host, port, user, password, and mountpoint arguments, and prints success.
- `loop()` sleeps 1 second.

**`enable_sd_csv.py`**

- `setup()` retries `begin()`, calls `enable_sd_csv(CSV_CFG_RMC | CSV_CFG_GGA)`, and keeps the existing commented `disable_sd_csv` and `delete_csv` lines next to that call.
- `loop()` sleeps 1 second.

## Error handling

Retry loops stay where the one-time calls are, inside `setup()`. A failed GGA update stays inside `loop()` and does not stop the process. No new error paths are added.

## Verification

Run `python -m py_compile` on the three example files. Hardware is not required for this change; the control flow and call arguments stay the same.
