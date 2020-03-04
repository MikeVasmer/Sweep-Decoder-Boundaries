import data_generator
import time

l_list = [4, 6, 8] # Linear length of lattice
cycles_list = [128] # N in the paper
p_list = [0.02, 0.025, 0.03, 0.035, 0.04] # Bit-flip error probabilities
# q = 0 # Measurement error probability
trials = 100 # Monte Carlo trials
lattice_type = 'rhombic_boundaries' # Can also choose rhombic_toric, cubic_toric or cubic_boundaries
sweep_schedules = ['alternating_XZ', 'alternating_XY', 'alternating_YZ', 'rotating_XZ', 'rotating_XY', 'rotating_YZ', 'random']
job_number = 1
greedy = False # Greedy sweep rule
correlated = False # Correlated errors
rates = [1, 3] # How many times to apply the sweep rule per syndrome measurement


for rate in rates:
    for l in l_list:
        timeouts = [32 * l] # How many times to apply rule in perfect decoding phase
        sweep_limit = (int) (round(pow(l, 0.5))) # How many sweeps before changing direction in error suppression phase
        for timeout in timeouts:
            for N in cycles_list:
                for p in p_list:
                    q = p # Measurement error probability
                    start_time = time.time()
                    data_generator.generate_data(
                        lattice_type, l, p, q, sweep_limit, sweep_schedules[0], timeout, N, trials, job_number, greedy, correlated, rate)
                    # Data will be saved as a json file
                    finish_time = round(time.time() - start_time, 2)
                    print('l={} p=q={} cycles={} trials={} job done in {} s'.format(
                        l, p, N, trials, finish_time))
                    job_number += 1
