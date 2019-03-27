import data_generator
import time

l_list = [8, 10, 14]
rounds_list = [0]
p_list = [0.15, 0.175, 0.2, 0.225]
q = 0
trials = 400
sweep_direction = 'xyz'
lattice_type = 'rhombic_boundaries'
sweep_schedule = 'alternating'
job_number = 1


for l in l_list:
    timeouts = [l, 8 * l, 64 * l, 128 * l]
    sweep_limit = l // 2
    for timeout in timeouts:
    # timeout = 8 * l * l
        for r in rounds_list:
            for p in p_list:
                # q = p
                start_time = time.time()
                data_generator.generate_data(
                    lattice_type, l, p, q, sweep_direction, sweep_limit, sweep_schedule, timeout, r, trials, job_number)
                finish_time = round(time.time() - start_time, 2)
                print('l={} p={} rounds={} trials={} timeout={} job done in {} s'.format(
                    l, p, r, trials, timeout, finish_time))
                job_number += 1
