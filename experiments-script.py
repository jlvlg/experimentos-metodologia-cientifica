# %%
import matplotlib.pyplot as plt
import statsmodels.api as sm
import pandas as pd
import subprocess
from pathlib import Path as p
import numpy as np
plt.close('all')

# %%
path = p().resolve()
p(path, 'inputs', 'insert').mkdir(parents=True, exist_ok=True)
p(path, 'inputs', 'start_remove').mkdir(parents=True, exist_ok=True)
p(path, 'inputs', 'end_remove').mkdir(parents=True, exist_ok=True)
p(path, 'outputs', 'insert').mkdir(parents=True, exist_ok=True)
p(path, 'outputs', 'start_remove').mkdir(parents=True, exist_ok=True)
p(path, 'outputs', 'end_remove').mkdir(parents=True, exist_ok=True)

# %%
def compile_script(tree):
    trees = p(path, 'trees')
    subprocess.run(
        [
            'gcc', '-g',
            p(trees, tree, f'{tree}.c'),
            p(trees, 'main.c'),
            '-include', p(trees, tree, f'{tree}.h'), 
            '-o', 
            p(trees, tree, f'{tree}.out')
        ]
    )
    
def generate_inputs(path, size):
    with open(p(path, 'insert', 'worst_case.in'), 'w') as insert:
        with open(p(path, 'start_remove', 'worst_case.in'), 'w') as start_remove:
            with open(p(path, 'end_remove', 'worst_case.in'), 'w') as end_remove:
                nums = list(range(1, size + 1))
                for num in nums:
                    insert.write(f'1 {num}\n')
                    start_remove.write(f'1 {num}\n')
                    end_remove.write(f'1 {num}\n')
                for num in nums:
                    start_remove.write(f'2 {num}\n')
                for num in reversed(nums):
                    end_remove.write(f'2 {num}\n')
                insert.write('0\n')
                start_remove.write('0\n')
                end_remove.write('0\n')
    with open(p(path, 'insert', 'average_case.in'), 'w') as insert:
        with open(p(path, 'start_remove', 'average_case.in'), 'w') as start_remove:
            with open(p(path, 'end_remove', 'average_case.in'), 'w') as end_remove:
                nums = np.random.permutation(size + 1)
                for num in nums:
                    insert.write(f'1 {num}\n')
                    start_remove.write(f'1 {num}\n')
                    end_remove.write(f'1 {num}\n')
                for num in nums:
                    start_remove.write(f'2 {num}\n')
                for num in reversed(nums):
                    end_remove.write(f'2 {num}\n')
                insert.write('0\n')
                start_remove.write('0\n')
                end_remove.write('0\n')
    with open(p(path, 'insert', 'best_case.in'), 'w') as insert:
        with open(p(path, 'start_remove', 'best_case.in'), 'w') as start_remove:
            with open(p(path, 'end_remove', 'best_case.in'), 'w') as end_remove:
                result = []
                div = 2
                for _ in range(20):
                    result.extend(round(mul * size / div) for mul in range(1, div, 2))
                    div *= 2
                result = list(dict.fromkeys(result))
                for num in result:
                    insert.write(f'1 {num}\n')
                    start_remove.write(f'1 {num}\n')
                    end_remove.write(f'1 {num}\n')
                for num in result:
                    start_remove.write(f'2 {num}\n')
                for num in reversed(result):
                    end_remove.write(f'2 {num}\n')
                insert.write('0\n')
                start_remove.write('0\n')
                end_remove.write('0\n')

def run_script(tree, input, output):
    trees = p(path, 'trees')
    with open(input) as input_file:
        with open(output, 'w') as output_file:
            subprocess.run(p(trees, tree, f'{tree}.out'), stdin=input_file, stdout=output_file)

# %%
compile_script('bst')
compile_script('avl')
compile_script('rb')

# %%
generate_inputs(f'{path}/inputs', 200000)

# %%
run_script('bst', f'{path}/inputs/insert/worst_case.in', f'{path}/outputs/insert/bst_worst_case.csv')
run_script('bst', f'{path}/inputs/insert/average_case.in', f'{path}/outputs/insert/bst_average_case.csv')
run_script('bst', f'{path}/inputs/insert/best_case.in', f'{path}/outputs/insert/bst_best_case.csv')
run_script('bst', f'{path}/inputs/start_remove/worst_case.in', f'{path}/outputs/start_remove/bst_worst_case.csv')
run_script('bst', f'{path}/inputs/start_remove/average_case.in', f'{path}/outputs/start_remove/bst_average_case.csv')
run_script('bst', f'{path}/inputs/start_remove/best_case.in', f'{path}/outputs/start_remove/bst_best_case.csv')
run_script('bst', f'{path}/inputs/end_remove/worst_case.in', f'{path}/outputs/end_remove/bst_worst_case.csv')
run_script('bst', f'{path}/inputs/end_remove/average_case.in', f'{path}/outputs/end_remove/bst_average_case.csv')
run_script('bst', f'{path}/inputs/end_remove/best_case.in', f'{path}/outputs/end_remove/bst_best_case.csv')

# %%
run_script('avl', f'{path}/inputs/insert/worst_case.in', f'{path}/outputs/insert/avl_worst_case.csv')
run_script('avl', f'{path}/inputs/insert/average_case.in', f'{path}/outputs/insert/avl_average_case.csv')
run_script('avl', f'{path}/inputs/insert/best_case.in', f'{path}/outputs/insert/avl_best_case.csv')
run_script('avl', f'{path}/inputs/start_remove/worst_case.in', f'{path}/outputs/start_remove/avl_worst_case.csv')
run_script('avl', f'{path}/inputs/start_remove/average_case.in', f'{path}/outputs/start_remove/avl_average_case.csv')
run_script('avl', f'{path}/inputs/start_remove/best_case.in', f'{path}/outputs/start_remove/avl_best_case.csv')
run_script('avl', f'{path}/inputs/end_remove/worst_case.in', f'{path}/outputs/end_remove/avl_worst_case.csv')
run_script('avl', f'{path}/inputs/end_remove/average_case.in', f'{path}/outputs/end_remove/avl_average_case.csv')
run_script('avl', f'{path}/inputs/end_remove/best_case.in', f'{path}/outputs/end_remove/avl_best_case.csv')

# %%
run_script('rb', f'{path}/inputs/insert/worst_case.in', f'{path}/outputs/insert/rb_worst_case.csv')
run_script('rb', f'{path}/inputs/insert/average_case.in', f'{path}/outputs/insert/rb_average_case.csv')
run_script('rb', f'{path}/inputs/insert/best_case.in', f'{path}/outputs/insert/rb_best_case.csv')
run_script('rb', f'{path}/inputs/start_remove/worst_case.in', f'{path}/outputs/start_remove/rb_worst_case.csv')
run_script('rb', f'{path}/inputs/start_remove/average_case.in', f'{path}/outputs/start_remove/rb_average_case.csv')
run_script('rb', f'{path}/inputs/start_remove/best_case.in', f'{path}/outputs/start_remove/rb_best_case.csv')
run_script('rb', f'{path}/inputs/end_remove/worst_case.in', f'{path}/outputs/end_remove/rb_worst_case.csv')
run_script('rb', f'{path}/inputs/end_remove/average_case.in', f'{path}/outputs/end_remove/rb_average_case.csv')
run_script('rb', f'{path}/inputs/end_remove/best_case.in', f'{path}/outputs/end_remove/rb_best_case.csv')


