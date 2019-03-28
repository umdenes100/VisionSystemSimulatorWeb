# (generated with --quick)

import argparse
import asyncio.events
import asyncio.subprocess
import collections
from typing import Any, Callable, Coroutine, Dict, Generator, IO, Iterable, Optional, Sized, Tuple, Type, TypeVar, Union

Executable = `namedtuple-Executable-command-working_directory`

BASE_DIR: str
REQUEST_TYPES: Dict[str, `namedtuple-Executable-command-working_directory`]
argparse: module
args: argparse.Namespace
json: module
loop: asyncio.events.AbstractEventLoop
os: module
parser: argparse.ArgumentParser
subprocess: module
uuid: module
websockets: module

_Tnamedtuple-Executable-command-working_directory = TypeVar('_Tnamedtuple-Executable-command-working_directory', bound=`namedtuple-Executable-command-working_directory`)

class `namedtuple-Executable-command-working_directory`(tuple):
    __slots__ = ["command", "working_directory"]
    __dict__: collections.OrderedDict[str, Any]
    _fields: Tuple[str, str]
    command: Any
    working_directory: Any
    def __getnewargs__(self) -> Tuple[Any, Any]: ...
    def __getstate__(self) -> None: ...
    def __init__(self, *args, **kwargs) -> None: ...
    def __new__(cls: Type[`_Tnamedtuple-Executable-command-working_directory`], command, working_directory) -> `_Tnamedtuple-Executable-command-working_directory`: ...
    def _asdict(self) -> collections.OrderedDict[str, Any]: ...
    @classmethod
    def _make(cls: Type[`_Tnamedtuple-Executable-command-working_directory`], iterable: Iterable, new = ..., len: Callable[[Sized], int] = ...) -> `_Tnamedtuple-Executable-command-working_directory`: ...
    def _replace(self: `_Tnamedtuple-Executable-command-working_directory`, **kwds) -> `_Tnamedtuple-Executable-command-working_directory`: ...


def create_subprocess_exec(program: Union[bytes, str], *args, stdin: Optional[Union[int, IO]] = ..., stdout: Optional[Union[int, IO]] = ..., stderr: Optional[Union[int, IO]] = ..., loop: asyncio.events.AbstractEventLoop = ..., limit: int = ..., **kwds) -> Generator[Any, None, asyncio.subprocess.Process]: ...
def get_event_loop() -> asyncio.events.AbstractEventLoop: ...
def middleware(websocket, path) -> Coroutine[Any, Any, None]: ...
def namedtuple(typename: str, field_names: Union[str, Iterable[str]], *, verbose: bool = ..., rename: bool = ...) -> type: ...
def process_command(command, working_directory, data = ...) -> coroutine: ...
