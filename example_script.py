import data_generator
import time
import math

l_list = [14, 18, 24, 32] # Linear length of lattice
cycles_list = [1024] # N in the paper
p_list = [0.0207, 0.0208, 0.0209, 0.021, 0.0211, 0.0212] # Bit-flip error probabilities
# q = 0 # Measurement error probability
trials = 100 # Monte Carlo trials
lattice_type = 'rhombic_toric' # Can also choose rhombic_toric, cubic_toric or cubic_boundaries
sweep_schedules = ['alternating_XZ', 'alternating_XY', 'alternating_YZ', 'rotating_XZ', 'rotating_XY', 'rotating_YZ', 'random']
job_number = 1
greedy = False # Greedy sweep rule
correlated = False # Correlated errors
rates = [1] # How many times to apply the sweep rule per syndrome measurement


for rate in rates:
    for l in l_list:
        timeouts = [32 * l] # How many times to apply rule in perfect decoding phase
        # sweep_limit = (int) (round(pow(l, 0.5))) # How many sweeps before changing direction in error suppression phase
        sweep_limit = (int) (round(math.log(l))) # How many sweeps before changing direction in error suppression phase
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
