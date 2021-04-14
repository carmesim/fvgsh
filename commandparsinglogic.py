from typing import Union, Optional
import os

# An abstraction over an file descriptor
# Either it contains the number of a file descriptor
# or a path name
FileDescriptor = Union[int, str]

class Command():
    """Command represents an arbitrary shellcommand"""
    # The /usr/bin/ or /bin/ binary to be executed
    binary: str

    # What `binary` will read from
    input: FileDescriptor

    # What `binary` will write to
    output: FileDescriptor

    def __init__(self, binary: str, inp: FileDescriptor, outp: FileDescriptor):
        self.binary = binary
        self.input = inp
        self.output = outp

    def __str__(self):
        return f"Command [\n\tbinary = {self.binary}\n\tinput = {self.input}\n\toutput = {self.output}\n]"

    def __repr__(self):
        return str(self)


def parse_command(command: list[str]) -> Command:

    input: Optional[FileDescriptor] = None
    output: Optional[FileDescriptor] = None
    binary: Optional[str] = None

    for (idx, word) in enumerate(command):
        if word == ">":
            if output is not None:
                raise Exception(
                    "Malformed command: more than one output is not supported"
                )
            print(f"DBG: {command}, {idx}, {idx + 1}")
            output = command[idx + 1]  # Intentionally not checking for OOB
        if word == "<":
            if input is not None:
                raise Exception(
                    "Malformed command: more than one input is not supported"
                )
            input = command[idx - 1]  # Intentionally not checking for OOB

    if input is None:
        # File descriptor for stdin
        input = 0
    if output is None:
        # File descriptor for stdout
        output = 1
    if binary is None:
        binary = "temp"
    return Command(binary, input, output)



def split_piped_commands(tokens: list[str], pipe_positions: list[int]):
    initial_idx = 0
    final_idx = len(tokens) - 1
    if pipe_positions[-1] == final_idx:
        raise Exception("Malformed line: expected command after pipe operator")
    pipe_positions.append(final_idx + 1)

    commands = []
    for pos in pipe_positions:
        commands.append(
            tokens[initial_idx:pos]
        )
        initial_idx = pos + 1
    return commands


def parse_commands(tokens: list[str], pipe_positions: list[int]):
    commands = [tokens] if pipe_positions == [] else split_piped_commands(tokens, pipe_positions)
    return [parse_command(command) for command in commands]


if __name__ == '__main__':
    command = input("Input test command: ")
    words = command.split()

    # Primeiro passo: tentar encontrar posições de pipe
    pipes = [i for i, word in enumerate(words) if word.strip() == "|"]
    print(f"Tokens: {words}")
    print(f"Pipe positions: {pipes}")

    commands = parse_commands(
        tokens=words,
        pipe_positions=pipes
    )

    print(commands)
