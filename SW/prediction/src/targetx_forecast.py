import darts
import darts.models
import pandas as pd
import numpy as np
import json

#carga de datos
data = pd.read_csv('daemonlec5g_3days.csv', delimiter=';')
data = data[:-12]
data_a = data['aactivepower']
data_a = data_a[::5].to_numpy() #downsample 1min - 5min
data_a = darts.TimeSeries.from_values(data_a)

data_b = data['bactivepower']
data_b = data_b[::5].to_numpy() #downsample 1min - 5min
data_b = darts.TimeSeries.from_values(data_b)

data_c = data['cactivepower']
data_c = data_c[::5].to_numpy() #downsample 1min - 5min
data_c = darts.TimeSeries.from_values(data_c)

#min-max normalization
scaler = darts.dataprocessing.transformers.Scaler()
a_scaled = scaler.fit_transform(data_a)
b_scaled = scaler.fit_transform(data_b)
c_scaled = scaler.fit_transform(data_c)

#carga del modelo e inferencia
model = darts.models.BlockRNNModel.load('model/lstm_power_hour.pt')
prediction_a = model.predict(n=12, series=a_scaled)
prediction_b = model.predict(n=12, series=b_scaled)
prediction_c = model.predict(n=12, series=c_scaled)

#post-procesamiento y exportación del fichero
prediction_a = prediction_a.data_array()
prediction_a = prediction_a.data
prediction_a = np.squeeze(np.squeeze(prediction_a, 2), 1)
prediction_a = prediction_a.tolist()

prediction_b = prediction_b.data_array()
prediction_b = prediction_b.data
prediction_b = np.squeeze(np.squeeze(prediction_b, 2), 1)
prediction_b = prediction_b.tolist()

prediction_c = prediction_c.data_array()
prediction_c = prediction_c.data
prediction_c = np.squeeze(np.squeeze(prediction_c, 2), 1)
prediction_c = prediction_c.tolist()

#result = pd.DataFrame(prediction_a, prediction_b, prediction_c)
output_dict = {'aactivewpred': prediction_a, 'bactivewpred': prediction_b, 'cactivewpred': prediction_c}
output = json.dumps(output_dict)
print(output)

with open('prediction_1h.json', 'w', encoding='utf-8') as f:
    json.dump(output_dict, f, ensure_ascii=False, indent=4)