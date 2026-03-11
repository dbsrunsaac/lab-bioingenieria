
% PROCESAMIENTO DE  DATOS 
clc; close all; clear;

% Cargar la señal
data = readmatrix("semana_1_neftali_.xlsx");

% Separar columnas
x = data(:,1);   % Tiempo (s)
y = data(:,2);   % Amplitud (mV)

% Graficar la señal original
plot(x, y)
xlim([0 6])
title("Señal Diferencial")
xlabel("Tiempo (s)")
ylabel("Amplitud (mV)")
grid on
hold on
