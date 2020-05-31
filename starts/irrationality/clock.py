#!/usr/bin/env python3
import time

start_time = time.time()
print("Start")
while True:
    input("Press Enter to see elapsed time")
    elapsed_time = time.time() - start_time
    print("Elapsed time: ", elapsed_time)
