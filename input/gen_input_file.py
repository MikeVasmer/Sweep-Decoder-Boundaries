import csv
import subprocess

Ls = range(4, 42, 2)
# ps = [0.16, 0.17, 0.18, 0.19]
ps = [0.19]
cycles = 0
trials = 1e2
trials = int(trials)

job = 0

with open('20_05_20.csv', 'w') as csv_file:
    writer = csv.writer(csv_file, delimiter=',')
    writer.writerow(['L', 'p', 'cycles', 'trials', 'job_number'])
    for L in Ls:
        for p in ps:
            writer.writerow([L, p, cycles, trials, job])
            job += 1