import data_generator
import time

l_list = [6, 8, 10]
rounds_list = [16]
p_list = [0.01, 0.014, 0.02, 0.028, 0.04, 0.056]
# q = 0
trials = 1000
sweep_direction = 'NaN'
lattice_type = 'rhombic boundaries'
job_number = 1

for l in l_list:
    for r in rounds_list:
        for p in p_list:
            start_time = time.time()
            data_generator.generate_data(l, p, p, sweep_direction, r, trials, job_number, lattice_type)
            finish_time = round(time.time() - start_time, 2)
            print('l={} p={} rounds={} trials={} job done in {} s'.format(l, p, r, trials, finish_time))