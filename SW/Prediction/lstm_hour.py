import darts.dataprocessing
import darts.dataprocessing.transformers
import darts.metrics
import darts.models
import torch
import numpy as np
import pandas as pd
import sklearn
import matplotlib.pyplot as plt
import darts
import os
import datetime


#función para codificar datos cíclicos (hora, día, mes, etc)
#fuente: https://www.kaggle.com/code/avanwyk/encoding-cyclical-features-for-deep-learning
def encode(data, max_val):
    sin = np.sin(2 * np.pi * data/max_val)
    cos = np.cos(2 * np.pi * data/max_val)
    encoded_data = np.vstack((sin, cos))
    return encoded_data
#########################################################################################


#carga de datos
data_list = os.listdir('data')
data = []
timestamp = []
for day in data_list:
    #series = pd.read_csv(os.path.join('data', day), delimiter=' ', usecols=['#Timestamp', 'E_imp(Wh)'], index_col=['#Timestamp'])
    series = pd.read_csv(os.path.join('data', day), delimiter=' ', usecols=['#Timestamp', 'A_pwrFT(kW)'], index_col=['#Timestamp'])
    timestamp_day = pd.read_csv(os.path.join('data', day), delimiter=' ', usecols=['#Timestamp'])
    data.append(series)
    timestamp.append(timestamp_day)

data = pd.concat(data)
data = data[::60].to_numpy() #downsample data: 5s - 5min
timestamp = pd.concat(timestamp)
timestamp = timestamp[::60].to_numpy() #downsample data: 5s - 5min

#obtención del día de la semana en int(0-6) y hora (0-24)
weekday_list = []
hour_list = []
for i in range(len(timestamp)):
    date = datetime.datetime.fromtimestamp(timestamp[i][0])
    date_weekday = date.weekday()
    date_hour = date.hour
    weekday_list.append(date_weekday)
    hour_list.append(date_hour)

weekday_list = np.array(weekday_list)
hour_list = np.array(hour_list)
weekday = encode(weekday_list, 6)
hour = encode(hour_list, 24)


data = darts.TimeSeries.from_values(data)
train_data, test_data = data.split_after(0.8)

#min-max normalization
scaler = darts.dataprocessing.transformers.Scaler()
train_scaled = scaler.fit_transform(train_data)
test_scaled = scaler.transform(test_data)
data_scaled = scaler.transform(data)
train_scaled.plot(label='train')
test_scaled.plot(label='test')

#entrenamiento del modelo
model = darts.models.BlockRNNModel(
    input_chunk_length=576, 
    output_chunk_length=12, 
    model='LSTM',
    hidden_dim=50,
    n_rnn_layers=2,
    hidden_fc_sizes=[100],
    n_epochs=50,
    batch_size=32,
    model_name='targetx_lstm_power')

model.fit(train_scaled, 
          val_series=test_scaled,  
          verbose=True)
model.save('model/lstm_power_hour.pt')