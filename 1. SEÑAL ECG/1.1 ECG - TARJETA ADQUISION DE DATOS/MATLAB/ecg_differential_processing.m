clc, clear;
% Lectura del archivo de datos
data_ecg = readmatrix("DATOS_ECG/ECG_differential.csv");
signal_ecg = data_ecg(:, 2);

% Datos de lectura
fs = 1000;
ts = 1/fs;
t = 0 : ts : (length(data_ecg(:, 1)) - 1)/fs;

% Graficar los resultados
figure;
plot(t, signal_ecg);
xlabel("Tiempo [s]");
ylabel("Amplitud");

% Mostrar resultados
T_ecg = 0.857;
F_ecg = 1/T_ecg;

disp(["Periodo ECG:", T_ecg]);
disp(["Frecuencia ECG", F_ecg]);
disp(["Latidos/Minuto: ", F_ecg*60]);