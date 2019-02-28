# VisionSystemSimulatorWeb

* This guide assumes that you have the command `pip` aliased to `pip3`.
* This guide assumes that you have the command `python` aliased to `python3`.

## Middleware

To use middleware, you must first install the websockets library.

```bash
pip install websockets
```

To run the server, simply run -

```bash
python middleware.py
```

This will create a simple server that passes messages between the website and the "C program".

The "C program" sleeps for a random amount of time between 0 and 2 seconds, and then returns the same input that was passed into it but with the associated delay tagged onto the beginning.

It's just meant to simulate what working with the actual C program will be. The middleware will also print out the message it plans to send back to the client.

At the start of `middleware.py`, there's a variable called `COMMAND`. Once you have a C program set up, simply change `COMMAND` to however you call it. The program passes input in through `stdin` and expects a response through `stdout`.

## Client

To run the client, simply run the command - 

```bash
cd VisionSystemSimulatorWeb
python -m http.server
```

In order to see what the client is receiving, go to `localhost:8000`, right-click, click `Inspect Element` and then `Console`.
