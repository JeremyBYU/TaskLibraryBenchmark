import json
import argparse
import math
import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt

sns.set()

TIME_COLUMN = 'real_time'

def partition_name(name:str):
    values = name.split('/')
    benchmark_all = values[0].split('_')
    benchmark_name = benchmark_all[1]
    task_library = benchmark_all[2]
    problem_size = int(values[1])
    threads = int(values[2])

    return dict(benchmark_name=benchmark_name, task_library=task_library, problem_size=problem_size, threads=threads)

def create_df(fpath:str):
    with open(fpath) as f:
        data = json.load(f)
    
    bench = data['benchmarks']
    df = pd.DataFrame.from_records(bench)
    # print(df)
    # return
    names = df['run_name'].unique()
    aggregates = ['mean', 'median', 'stddev']
    records = []
    for name in names:
        record = partition_name(name)
        for aggregate_name in aggregates:
            aggregate_row = df.loc[(df.run_name == name) & (df.aggregate_name == aggregate_name)]
            record[aggregate_name] = float(aggregate_row.iloc[0]['real_time'])   
        records.append(record)

    df_final = pd.DataFrame.from_records(records)

    return df_final

def plot_graphs(df:pd.DataFrame):
    df['problem_size'] = df['problem_size'].transform(lambda x: math.pow(2, x))

    df_renamed = df.rename(columns={"problem_size": "Problem Size", "mean": "Time (ms)", "benchmark_name": "Benchmark"})
    g = sns.relplot(x="Problem Size", y="Time (ms)", hue="task_library", col="threads", row="Benchmark", kind="line", data=df_renamed, marker='.')
    # sns.relplot(x="problem_size", y="mean", hue="task_library", row="benchmark_name", style="threads", kind="line", data=df)
    plt.show()

def parse_args():
    parser = argparse.ArgumentParser("Display graphs of google benchmark")
    parser.add_argument('-i', '--input', default='./results/tlb.json')

    args = parser.parse_args()
    return args

def main():
    args = parse_args()
    df = create_df(args.input)
    plot_graphs(df)

if __name__ == "__main__":
    main()