import data_generator
import time

l_list = [16, 20, 24, 28]
rounds_list = [0]
p_list_q0 = [0.05]
p_list_q16 = [0.032]
p_list_q = [0.017]
trials = 10
sweep_direction = 'xyz'
job_number = -1

for l in l_list:
    for r in rounds_list:
        if r == 0:
            p_list = p_list_q0
        elif r == 16:
            p_list = p_list_q16
        else:
            p_list = p_list_q
        for p in p_list:
            start_time = time.time()
            data_generator.generate_data(l, p, p, sweep_direction, r, trials, job_number)
            finish_time = round(time.time() - start_time, 2)
            print('l={} p={} rounds={} trials={} job done in {} s'.format(l, p, r, trials, finish_time))