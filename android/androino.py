
import android

BT_DEVICE_ID = ''

droid = android.Android()
droid.bluetoothConnect('00001101-0000-1000-8000-00805F9B34FB')

droid.webViewShow('file:///storage/emulated/0/sl4a/scripts/androino.html')
while True:
    result = droid.eventWaitFor('fetch_data').result['data']
    droid.bluetoothWrite(result)
    sensor_data = droid.bluetoothReadLine().result
    droid.eventClearBuffer()  # workaround for a bug in SL4A r4.
    droid.eventPost('display_data', sensor_data)


