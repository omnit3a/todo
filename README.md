# todo
A simple checklist program

# Usage
First, create an empty file that you will use as the checklist<br>
To use `todo`, enter `todo <name-of-checklist> <command> <argument>`

### List of Commands
- `-a`: Add `<argument>` to checklist
- `-r`: Remove `<argument>` from checklist
- `-c`: Check `<argument>` off list
- `-u`: Uncheck `<argument>` from list
- `-p`: Print checklist to `stdout`
- `-t`: Create checklist if it doesn't already exist
- `-q`: Check whether or not `<argument>` is checked

# Compilation
Run `make`

# Scripts
- `remtodo.sh`: List all the tasks left to complete in `<argument>` checklist
