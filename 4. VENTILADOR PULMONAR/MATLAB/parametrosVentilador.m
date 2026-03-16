% Reiniciando las variables

clc, clear, close all;

% Lectura de datos
data = readmatrix("DATOS/g1.csv");

% Procesamiento y filtrado
data = data(:,2:4);
data = rmmissing(data); % Eliminar las columnas NaN

fs = 61;
ts = 1/fs;
segundos = 2;
tiempoVisualizacion = fs*segundos;

t = 0 : ts : (length(data(1:tiempoVisualizacion, 1)) - 1)/fs;

mediaPresion = mean( data(1:tiempoVisualizacion, 1));
mediaFlujo = mean( data(1:tiempoVisualizacion, 2));
mediaVolumen = mean( data(1:tiempoVisualizacion, 3));

presion = (data(1:tiempoVisualizacion, 1) - 320)/18.6;
flujo = (data(1:tiempoVisualizacion, 2) - mediaFlujo)/50;
volumen = data(1:tiempoVisualizacion, 3)/8.42;

figure;
plot(t, presion);
xlabel("Tiempo [s]");
ylabel("Presión [mbar]");
title("Presión vs Tiempo");

figure;
plot(t, flujo);
xlabel("Tiempo [s]");
ylabel("Flujo [mL/min]");
title("Flujo vs Tiempo");

figure;
plot(t, volumen);
xlabel("Tiempo [s]");
ylabel("Volumen [mL]");
title("Volumen vs Tiempo");

figure;
plot(presion, volumen);
xlabel("Presion [mbar]");
ylabel("Volumen [mL]");
title("Volumen vs Presión");