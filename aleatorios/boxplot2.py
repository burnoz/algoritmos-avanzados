import os
import matplotlib.pyplot as plt

def readTxt(path):
    with open(path, 'r', encoding='utf-8') as f:
        vals = []
        for line in f:
            s = line.strip()
            if not s: 
                continue
            try:
                vals.append(float(s))
            except:
                pass
    return vals

def main():
    # Archivos RMSE por instancia (cada archivo contiene RMSE de un algoritmo sobre todas las corridas)
    instances = ['bcspwr01', 'bcspwr02', 'bcspwr03', 'ash85']
    
    # Leer datos por instancia
    data = []
    labels = []
    
    for inst in instances:
        rmse_file = f'{inst}_rmse_results.txt'
        if not os.path.exists(rmse_file):
            print(f"Warning: {rmse_file} not found")
            continue
            
        vals = readTxt(rmse_file)
        if not vals:
            print(f"Warning: {rmse_file} is empty")
            continue
        
        data.append(vals)
        labels.append(inst)
    
    if not data:
        print("No data to plot")
        return
    
    # Crear figura
    plt.figure(figsize=(10, 6))
    
    # Crear boxplot
    positions = list(range(len(labels)))
    bp = plt.boxplot(data, positions=positions, patch_artist=True, widths=0.6, tick_labels=labels)
    
    # Colorear cada instancia
    colors = ['#66c2a5', '#fc8d62', '#8da0cb', '#e78ac3']
    for i, patch in enumerate(bp['boxes']):
        patch.set_facecolor(colors[i % len(colors)])
        patch.set_alpha(0.8)
    
    # Configurar ejes
    plt.xlabel('Instancia', fontsize=12)
    plt.ylabel('RMSE', fontsize=12)
    plt.title('Comparación de RMSE por instancia', fontsize=14)
    plt.grid(axis='y', linestyle='--', alpha=0.4)
    
    plt.tight_layout()
    
    out = os.path.join(os.path.dirname(__file__), 'rmse_comparison_instances.png')
    plt.savefig(out, dpi=200)
    print(f"Saved: {out}")
    
    plt.show()
    plt.close()

if __name__ == "__main__":
    main()