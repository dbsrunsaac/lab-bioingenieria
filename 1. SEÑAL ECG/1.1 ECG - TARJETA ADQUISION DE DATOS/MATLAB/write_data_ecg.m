% Para hacer uso de este código es necesari contar con los datos a guardar
% como variables en el espacio de trabajo


datos = table(ecg_uc, ... 
    'VariableNames',{'Amplitud_ECG'});

nombre_archivo = 'ecg_filtered_uc.csv';

writetable(datos, nombre_archivo);

fprintf('Archivo "%s" guardado correctamente .\n', nombre_archivo);