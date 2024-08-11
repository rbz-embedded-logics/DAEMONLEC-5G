while [ 1 ]
do
        for i in {1..60}
        do
                /home/root/meter_read
                cat /tmp/data_energy.csv >> /home/root/appnn/data/daemonlec5g_3days.csv
                cat /tmp/data_energy.json | mosquitto_pub -d -q 1 -h mqtt.thingsboard.cloud -p 1883 -t v1/devices/me/telemetry -u "71a9fy6gpwym29rf2rtg" --stdin-line
        done
        echo "###################### inference loop #########################"
        tail -n 4392 /home/root/appnn/data/daemonlec5g_3days.csv > /home/root/appnn/data/cut_daemonlec5g_3days.csv
        cp /home/root/appnn/data/cut_daemonlec5g_3days.csv /home/root/appnn/data/daemonlec5g_3days.csv
        python3 /home/root/appnn/targetx_forecast.py
        cat /home/root/appnn/prediction/prediction_1h.json | mosquitto_pub -d -q 1 -h mqtt.thingsboard.cloud -p 1883 -t v1/devices/me/telemetry -u "71a9fy6gpwym29rf2rtg" --stdin-line
done
