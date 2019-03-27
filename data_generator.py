import argparse
import subprocess
import os
import ast
import json
import time


def generate_data(lattice_type, l, p, q, sweep_direction, sweep_limit, sweep_schedule, timeout, rounds, trials, job_number):
    cwd = os.getcwd()
    build_directory = '{0}/{1}'.format(cwd, 'build')

    data = {}
    results = []
    successes = 0
    clear_syndromes = 0

    start_time = time.time()
    for _ in range(trials):
        result = subprocess.run(
            ['./RhombicSweep', str(l), str(p), str(q), sweep_direction, str(rounds), lattice_type, str(sweep_limit), sweep_schedule, str(timeout)], stdout=subprocess.PIPE, check=True, cwd=build_directory)
        # print(result.stdout.decode('utf-8'))
        result_list = ast.literal_eval(result.stdout.decode('utf-8'))
        # print(result_list)
        results.append(
            {'Success': result_list[0], 'Clear syndrome': result_list[1], 'Time (s)': result_list[2]})
        successes += result_list[0]
        clear_syndromes += result_list[1]
    elapsed_time = round(time.time() - start_time, 2)

    data['Results'] = results
    data['L'] = l
    data['p'] = p
    data['q'] = q
    data['Rounds'] = rounds
    data['Trials'] = trials
    data['Lattice Type'] = lattice_type
    data['Job RunTime (s)'] = elapsed_time
    data['Timeout'] = timeout
    data['Successes'] = successes
    data['Clear syndromes'] = clear_syndromes
    if lattice_type == 'rhombic_toric':
        data['Sweep direction'] = sweep_direction
        json_file = "L={}_N={}_p={:0.4f}_q={:0.4f}_trials={}_job={}.json".format(
            l, rounds, p, q, trials, job_number)
    elif lattice_type == "rhombic_boundaries":
        data['Sweep limit'] = sweep_limit
        data['Sweep schedule'] = sweep_schedule
        json_file = "L={}_N={}_p={:0.4f}_q={:0.4f}_timeout={}_trials={}_job={}.json".format(
            l, rounds, p, q, timeout, trials, job_number)

    with open(json_file, 'w') as output:
        json.dump(data, output)

    # print(results)
    # print(len(results))


if __name__ == "__main__":

    parser = argparse.ArgumentParser(
        description="Sweep rule decoder for rhombic dodecahedral kitaev codes.",
        formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    parser.add_argument("lattice_type", type=str,
                        default="rhombic toric", help="the lattice type [rhombic_boundaries|rhombic_toric]")
    parser.add_argument("l", type=int,
                        help="the length of the l by l by l lattice")
    parser.add_argument("p", type=float,
                        help="the data qubit error probability")
    parser.add_argument("q", type=float,
                        help="the measurement error probability")
    parser.add_argument("rounds", type=int,
                        help="the number of rounds of error correction")
    parser.add_argument("trials", type=int,
                        help="the number of trials")
    parser.add_argument("--sweep_limit", default='l/2',
                        help="the number of sweeps per direction")
    parser.add_argument("--sweep_schedule", type=str, default='alternating',
                        help="the sweep schedule [alternating|rotating|random]")
    parser.add_argument("--timeout", default='8*l^2',
                        help="the max number of sweeps before timeout (readout phase)")
    parser.add_argument("--sweep_direction", type=str, default='xyz',
                        help="the sweep direction")
    parser.add_argument("--job", type=int, default=-1,
                        help="the job number")

    args = parser.parse_args()
    lattice_type = args.lattice_type
    l = args.l
    p = args.p
    q = args.q
    rounds = args.rounds
    trials = args.trials
    sweep_limit = args.sweep_limit
    if sweep_limit == 'l/2':
        sweep_limit = l // 2
    sweep_schedule = args.sweep_schedule
    timeout = args.timeout
    if timeout == '8*l^2':
        timeout = 8 * l * l
    sweep_direction = args.sweep_direction
    job_number = args.job

    print(timeout,sweep_direction,sweep_schedule,sweep_limit,job_number)

    generate_data(lattice_type, l, p, q, sweep_direction, sweep_limit,
                  sweep_schedule, timeout, rounds, trials, job_number)
