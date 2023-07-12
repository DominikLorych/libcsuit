#!/usr/bin/python3
import os
import subprocess

testfile_path = "testfiles/"
bin_path = "bin/"

bin = "suit_manifest_process"
print("Running valgrind massif profiling on ", bin)
directory = os.fsencode(testfile_path)
mem_heap_b = []
mem_heap_extra_b = []
mem_stack_b = []
total = []
i = 0

for file in os.listdir(directory):
    filename = os.fsdecode(file)
    if not filename.endswith(".cbor"):
        continue
    else:
        if (filename.__eq__("suit_manifest_expID.cbor")):
            print("Skipping file {}, it currently does not work!".format(filename))
            continue
        print("  File: {}".format(filename))
        command = "valgrind -q --tool=massif --heap=yes --stacks=yes --massif-out-file=massif.out {}{} {}{}".format(bin_path, bin, testfile_path, filename)
        command_split = command.split()
        proc = subprocess.Popen(command_split, shell=False, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        out, err = proc.communicate()
        if (proc.returncode != 0):
            print("Error: Command '{}' failed with error code {}, Exiting!".format(command, proc.returncode))
            print("stdout: {}".format(out.decode('ascii')))
            print("stderr: {}".format(err.decode('ascii')))
            continue
        stream = os.popen('cat massif.out | grep "mem_heap_B" | grep -G -o "[0-9]*" | sort -n | tail -1')
        mem_heap_b.append(int(stream.read()))
        stream = os.popen('cat massif.out | grep "mem_heap_extra_B" | grep -G -o "[0-9]*" | sort -n | tail -1')
        mem_heap_extra_b.append(int(stream.read()))
        stream = os.popen('cat massif.out | grep "mem_stacks_B" | grep -G -o "[0-9]*" | sort -n | tail -1')
        mem_stack_b.append(int(stream.read()))
        total.append(mem_heap_b[i] + mem_heap_extra_b[i] + mem_stack_b[i])
        print("    Memory Usage (B): Total: {:,} | Heap: {:,} | Heap Extra: {:,} | Stack: {:,}".format(total[i], mem_heap_b[i], mem_heap_extra_b[i], mem_stack_b[i]))
        i+=1

print("Summary for {}:".format(bin))
print("  Average Memory Usage (B): Total: {:,.0f} | Heap: {:,.0f} | Heap Extra: {:,.0f} | Stack: {:,.0f}".format(sum(total)/len(total), sum(mem_heap_b)/len(mem_heap_b), sum(mem_heap_extra_b)/len(mem_heap_extra_b), sum(mem_stack_b)/len(mem_stack_b)))
print("  Maximum Memory Usage (B): Total: {:,.0f} | Heap: {:,.0f} | Heap Extra: {:,.0f} | Stack: {:,.0f}".format(max(total), max(mem_heap_b), max(mem_heap_extra_b), max(mem_stack_b)))

bin = "suit_manifest_parser"
print("Running valgrind massif profiling on %s", bin)
mem_heap_b = []
mem_heap_extra_b = []
mem_stack_b = []
total = []
i = 0

for file in os.listdir(directory):
    filename = os.fsdecode(file)
    if not filename.endswith(".cbor"):
        continue
    else:
        if (filename.__eq__("suit_manifest_expID.cbor")):
            print("Skipping file {}, it currently does not work!".format(filename))
            continue
        print("  File: {}".format(filename))
        command = "valgrind -q --tool=massif --heap=yes --stacks=yes --massif-out-file=massif.out {}{} {}{}".format(bin_path, bin, testfile_path, filename)
        command_split = command.split()
        proc = subprocess.Popen(command_split, shell=False, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        out, err = proc.communicate()
        if (proc.returncode != 0):
            print("Error: Command '{}' failed with error code {}, Exiting!".format(command, proc.returncode))
            print("stdout: {}".format(out.decode('ascii')))
            print("stderr: {}".format(err.decode('ascii')))
            continue
        stream = os.popen('cat massif.out | grep "mem_heap_B" | grep -G -o "[0-9]*" | sort -n | tail -1')
        mem_heap_b.append(int(stream.read()))
        stream = os.popen('cat massif.out | grep "mem_heap_extra_B" | grep -G -o "[0-9]*" | sort -n | tail -1')
        mem_heap_extra_b.append(int(stream.read()))
        stream = os.popen('cat massif.out | grep "mem_stacks_B" | grep -G -o "[0-9]*" | sort -n | tail -1')
        mem_stack_b.append(int(stream.read()))
        total.append(mem_heap_b[i] + mem_heap_extra_b[i] + mem_stack_b[i])
        print("    Memory Usage (B): Total: {:,} | Heap: {:,} | Heap Extra: {:,} | Stack: {:,}".format(total[i], mem_heap_b[i], mem_heap_extra_b[i], mem_stack_b[i]))
        i+=1

print("Summary for {}:".format(bin))
print("  Average Memory Usage (B): Total: {:,.0f} | Heap: {:,.0f} | Heap Extra: {:,.0f} | Stack: {:,.0f}".format(sum(total)/len(total), sum(mem_heap_b)/len(mem_heap_b), sum(mem_heap_extra_b)/len(mem_heap_extra_b), sum(mem_stack_b)/len(mem_stack_b)))
print("  Maximum Memory Usage (B): Total: {:,.0f} | Heap: {:,.0f} | Heap Extra: {:,.0f} | Stack: {:,.0f}".format(max(total), max(mem_heap_b), max(mem_heap_extra_b), max(mem_stack_b)))

bin = "suit_manifest_encode"
i = 0
mem_heap_b = []
mem_heap_extra_b = []
mem_stack_b = []
total = []
print("Running valgrind massif profiling on %s", bin)
command = "valgrind -q --tool=massif --heap=yes --stacks=yes --massif-out-file=massif.out {}{} {}{}".format(bin_path, bin, testfile_path, filename)
proc = subprocess.Popen([command], shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
out, err = proc.communicate()
if (proc.returncode != 0):
    print("Error: Command '{}' failed with error code {}, Exiting!".format(command, proc.returncode))
    print("stdout: {}".format(out.decode('ascii')))
    print("stderr: {}".format(err.decode('ascii')))
    exit()
stream = os.popen('cat massif.out | grep "mem_heap_B" | grep -G -o "[0-9]*" | sort -n | tail -1')
mem_heap_b.append(int(stream.read()))
stream = os.popen('cat massif.out | grep "mem_heap_extra_B" | grep -G -o "[0-9]*" | sort -n | tail -1')
mem_heap_extra_b.append(int(stream.read()))
stream = os.popen('cat massif.out | grep "mem_stacks_B" | grep -G -o "[0-9]*" | sort -n | tail -1')
mem_stack_b.append(int(stream.read()))
total.append(mem_heap_b[i] + mem_heap_extra_b[i] + mem_stack_b[i])
print("    Memory Usage (B): Total: {:,} | Heap: {:,} | Heap Extra: {:,} | Stack: {:,}".format(total[i], mem_heap_b[i], mem_heap_extra_b[i], mem_stack_b[i]))