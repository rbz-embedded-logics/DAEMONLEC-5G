import darts
import darts.models
import pandas as pd
import numpy as np

#carga de datos
data = pd.read_csv('data')
############################################## pre-procesamiento
data = darts.TimeSeries.from_values(data)

#min-max normalization
scaler = darts.dataprocessing.transformers.Scaler()
data_scaled = scaler.fit_transform(data)

#carga del modelo e inferencia
model = darts.models.BlockRNNModel.load('model/lstm_power_hour.pt')
prediction = model.predict(n=12, series=data_scaled)

prediction = prediction.data_array()
prediction = prediction.data
prediction = np.squeeze(np.squeeze(prediction, 2), 1)
############################################## exportación 