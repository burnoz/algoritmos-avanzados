import sys
import os
import glob
import matplotlib.pyplot as plt

def readTxt(path):
    with open(path, 'r', encoding='utf-8') as f:
        vals = []

        for line in f:
            s = line.strip()
            
            if not s: 
                continue
            
            try:
                vals.append(int(s))
            
            except:
                try:
                    vals.append(int(float(s)))
                
                except:
                    pass
    return vals

def main():
    files = [["bcspwr01.mtx.rnd_output_first.txt", "bcspwr01.mtx.rnd_output_best.txt"], 
             ["bcspwr02.mtx.rnd_output_first.txt", "bcspwr02.mtx.rnd_output_best.txt"],
             ["bcspwr03.mtx.rnd_output_first.txt", "bcspwr03.mtx.rnd_output_best.txt"]]

    for fn in files:
        vals_1 = readTxt(fn[0])
        vals_2 = readTxt(fn[1])

        plt.figure(figsize=(12,6))

        box = plt.boxplot([vals_1, vals_2], patch_artist=True, tick_labels=['First', 'Best'], notch=False)
        colors = plt.cm.Set2.colors

        for i, patch in enumerate(box['boxes']):
            patch.set_facecolor(colors[i % len(colors)])
            patch.set_alpha(0.9)

        plt.ylabel('Valor')
        inst_name = os.path.basename(fn[0]).replace("_mtx.rnd_output_first.txt","").replace("_output_first.txt","")
        plt.title(inst_name)
        plt.grid(axis='y', linestyle='--', alpha=0.4)

        out = os.path.join(os.path.dirname(__file__), f'{inst_name}_boxplot.png')
        plt.tight_layout()
        plt.savefig(out, dpi=200)

        plt.show()
        plt.close()

if __name__ == "__main__":
    main()