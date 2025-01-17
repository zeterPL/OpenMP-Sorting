#!/bin/bash
#SBATCH --job-name=quicksort_test       # Nazwa zadania
#SBATCH --output=output_%j.txt          # Plik wyjściowy (logi)
#SBATCH --error=error_%j.txt            # Plik błędów
#SBATCH --ntasks=1                      # Liczba zadań
#SBATCH --cpus-per-task=6              # Liczba rdzeni na zadanie

# Uruchomienie programu z srun
srun ./main
