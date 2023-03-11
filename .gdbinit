# https://github.com/nlohmann/json/blob/develop/tools/gdb_pretty_printer/nlohmann-json.py
source tools/nlohmann-json.py

python
import sys
sys.path.insert(1, 'tools/Boost-Pretty-Printer')
print("importing boost...")
import boost
boost.register_printers()
end