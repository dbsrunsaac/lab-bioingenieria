clc, clear, close all;
% Lectura del archivo de datos
data_ecg = readmatrix("DATOS_ECG/ECG_ruido_common.csv");
signal_ecg = data_ecg(:, 2);

% Datos de lectura
fs = 1000;
ts = 1/fs;
t = 0 : ts : (length(data_ecg(:, 1)) - 1)/fs;

% Limitar la señal a 2 periodos de la señal
limit_inf = 540;
limit_sup= 1173;
t2 = 0 : ts : (length(data_ecg(limit_inf:limit_sup, 1)) -1)/fs;
amp_ecg = data_ecg(limit_inf:limit_sup, 2);

% Filtrado señal resultante
fc = 30;
wn = fc/(fs/2);
[b, a] = butter(4, wn, 'low');
ecg_low = filtfilt(b, a, amp_ecg);
ecg_low = ecg_low +  + 0.01809 + 0.01661;

figure;
hold on;
etiquetas(1) = "";
j = 1;
for i = 1 : 5 : 60 
    wn = i/(fs/2);
    [b, a] = butter(4, wn, 'low');
    ecg_low_1=filtfilt(b, a, amp_ecg);
    etiquetas(j) = "Freq: " + num2str(i) + " [Hz]"; 
    plot(t2, ecg_low_1);
    j = j + 1;
end
title("ECG - Modo Común Filtrada");
legend(etiquetas, 'Location', 'best');
xlabel("Tiempo [s]");
ylabel("Amplitud");
hold off;

% Escalar la señal filtrada para ajustarla al uC
ecg_uc(1) = 0;
for i = 1 : length(ecg_low)
    ecg_uc(i) = round(ecg_low(i)*255/0.1184);
end


% Mostrar resultados
T_ecg = 0.666;
F_ecg = 1/T_ecg;

disp(["Periodo ECG:", T_ecg]);
disp(["Frecuencia ECG", F_ecg]);
disp(["Latidos/Minuto: ", F_ecg*60]);

% Graficar los resultados
figure;
plot(t, signal_ecg);
title("ECG - Modo Común");
xlabel("Tiempo [s]");
ylabel("Amplitud");

figure;
plot(t2, amp_ecg);
title("ECG - Modo Común");
xlabel("Tiempo [s]");
ylabel("Amplitud");

figure;
plot(t2, ecg_low);
title("ECG - Modo Común Filtrada");
xlabel("Tiempo [s]");
ylabel("Amplitud");

figure;
plot(t2, ecg_uc);
title("ECG - Modo Común Filtrada - Escalada");
xlabel("Tiempo [s]");
ylabel("Amplitud");