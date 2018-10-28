import argparse
import subprocess
import os
import ast
import json
import time


def generate_data(l, p, q, sweep_direction, rounds, trials, job_number, lattice_type):
    cwd = os.getcwd()
    build_directory = '{0}/{1}'.format(cwd, 'build')

    data = {}
    results = []

    start_time = time.time()
    for _ in range(trials):
        result = subprocess.run(
            ['./RhombicSweep', str(l), str(p), str(q), sweep_direction, str(rounds), lattice_type], stdout=subprocess.PIPE, check=True, cwd=build_directory)
        # print(result.stdout.decode('utf-8'))
        result_list = ast.literal_eval(result.stdout.decode('utf-8'))
        # print(result_list)
        results.append(
            {'Success': result_list[0], 'Clear syndrome': result_list[1], 'Time (s)': result_list[2]})
    elapsed_time = round(time.time() - start_time, 2)

    data['Results'] = results
    data['L'] = l
    data['p'] = p
    data['q'] = q
    data['Rounds'] = rounds
    data['Trials'] = trials
    if lattice_type == 'rhombic toric':
        data['Sweep direction'] = sweep_direction
    data['Lattice Type'] = lattice_type
    data['Job RunTime'] = elapsed_time
    if lattice_type == 'rhombic toric':
        json_file = "L={}_rounds={}_p={:0.4f}_q={:0.4f}_trials={}_sweepDir={}_latticeType={}_job={}.json".format(
            l, rounds, p, q, trials, sweep_direction, lattice_type, job_number)
    elif lattice_type == "rhombic boundaries":
        json_file = "L={}_rounds={}_p={:0.4f}_q={:0.4f}_trials={}_latticeType={}_job={}.json".format(
            l, rounds, p, q, trials, lattice_type, job_number)

    with open(json_file, 'w') as output:
        json.dump(data, output)

    # print(results)
    # print(len(results))


if __name__ == "__main__":

    parser = argparse.ArgumentParser(
        description="Sweep rule decoder for rhombic dodecahedral kitaev codes.",
        formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    parser.add_argument("l", type=int,
                        help="the length of the l by l by l lattice")
    parser.add_argument("p", type=float, default=0.05,
                        help="the data qubit error probability")
    parser.add_argument("q", type=float, default=0.05,
                        help="the measurement error probability")
    parser.add_argument("rounds", type=int, default=1,
                        help="the number of rounds of error correction")
    parser.add_argument("trials", type=int, default=10,
                        help="the number of trials")
    parser.add_argument("lattice_type", type=str,
                        default="rhombic toric", help="the lattice type")
    parser.add_argument("--job", type=int, default=-1,
                        help="the job number")

    args = parser.parse_args()
    l = args.l
    p = args.p
    q = args.q
    rounds = args.rounds
    trials = args.trials
    job_number = args.job
    sweep_direction = 'xyz'
    lattice_type = args.lattice_type

    generate_data(l, p, q, sweep_direction, rounds,
                  trials, job_number, lattice_type)
