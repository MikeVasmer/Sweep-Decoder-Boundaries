import argparse
import subprocess
import os
import ast
import json
import time


def snake_case_to_CamelCase(word):
    return ''.join(x.capitalize() or '_' for x in word.split('_'))


def generate_data(lattice_type, l, p, q, sweep_direction, sweep_limit, sweep_schedule, timeout, cycles, trials, job_number, greedy):
    cwd = os.getcwd()
    build_directory = '{0}/{1}'.format(cwd, 'build')

    data = {}
    results = []
    successes = 0
    clear_syndromes = 0

    start_time = time.time()
    for _ in range(trials):
        result = subprocess.run(
            ['./SweepDecoder', str(l), str(p), str(q), sweep_direction, str(cycles), lattice_type, str(sweep_limit), sweep_schedule, str(timeout), str(greedy).lower()], stdout=subprocess.PIPE, check=True, cwd=build_directory)
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
    data['Cycles'] = cycles
    data['Trials'] = trials
    data['Lattice Type'] = lattice_type
    data['Job RunTime (s)'] = elapsed_time
    data['Timeout'] = timeout
    data['Successes'] = successes
    data['Clear syndromes'] = clear_syndromes
    data['Greedy'] = greedy

    if lattice_type == 'rhombic_toric':
        data['Sweep direction'] = sweep_direction
        # lattice_type = lattice_type.replace('_', '')
        lattice_type = snake_case_to_CamelCase(lattice_type)
        json_file = "L={}_N={}_p={:0.4f}_q={:0.4f}_direction={}_trials={}_lattice={}_job={}.json".format(
            l, cycles, p, q, sweep_direction, trials, lattice_type, job_number)
    elif lattice_type == "rhombic_boundaries" or lattice_type == "cubic_boundaries":
        data['Sweep limit'] = sweep_limit
        data['Sweep schedule'] = sweep_schedule
        # lattice_type = lattice_type.replace('_', '')
        lattice_type = snake_case_to_CamelCase(lattice_type)
        sweep_schedule = snake_case_to_CamelCase(sweep_schedule)
        json_file = "L={}_N={}_p={:0.4f}_q={:0.4f}_schedule={}_sweepLimit={}_timeout={}_lattice={}_trials={}_job={}.json".format(
            l, cycles, p, q, sweep_schedule, sweep_limit, timeout, lattice_type, trials, job_number)

    with open(json_file, 'w') as output:
        json.dump(data, output)

    # print(results)
    # print(len(results))


if __name__ == "__main__":

    parser = argparse.ArgumentParser(
        description="Sweep rule decoder for 3D surface codes.")
    parser.add_argument("lattice_type", type=str, default="rhombic_toric", help="lattice type",
                        choices=['cubic_boundaries', 'rhombic_boundaries', 'rhombic_toric'])
    parser.add_argument("l", type=int,
                        help="length of l by l by l lattice")
    parser.add_argument("p", type=float,
                        help="data qubit error probability")
    parser.add_argument("q", type=float,
                        help="measurement error probability")
    parser.add_argument("cycles", type=int,
                        help="number of error correction cycles")
    parser.add_argument("trials", type=int,
                        help="number of trials")
    parser.add_argument("--sweep_limit", type=int,
                        help="number of sweeps per direction in active phase (default: l/2)")
    parser.add_argument("--sweep_schedule", type=str, default='random', choices=[
                        'rotating_XY', 'alternating_XY', 'rotating_XZ', 'alternating_XZ', 'rotating_YZ', 'alternating_YZ', 'random'], help="sweep direction schedule (default: random)")
    parser.add_argument("--timeout", type=int,
                        help="max number of sweeps before timeout in readout phase (default: 128*l)")
    parser.add_argument("--sweep_direction", type=str, default='xyz',
                        help="sweep direction (default: xyz)")
    parser.add_argument("--greedy", action='store_true',
                        help="use the greedy sweep rule (default : True)")
    parser.add_argument("--job", type=int, default=-1,
                        help="job number (default: -1)")

    args = parser.parse_args()
    lattice_type = args.lattice_type
    l = args.l
    p = args.p
    q = args.q
    cycles = args.cycles
    trials = args.trials
    sweep_limit = args.sweep_limit
    if sweep_limit == None:
        sweep_limit = int(round(pow(l, 0.5)))
    sweep_schedule = args.sweep_schedule
    timeout = args.timeout
    if timeout == None:
        timeout = 32 * l
    sweep_direction = args.sweep_direction
    job_number = args.job
    greedy = args.greedy

    generate_data(lattice_type, l, p, q, sweep_direction, sweep_limit,
                  sweep_schedule, timeout, cycles, trials, job_number, greedy)
