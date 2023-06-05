# -*- coding: utf-8 -*-
"""
@author: Professor
"""

"""
{
"raw_values":[
    {
        "Temperature":32.47,
        "Humidity":59.26,
        "MQ7_analog":2867.0,
        "MQ9_analog":1539.0,
        "MG811_analog":4654.0,
        "MQ135_analog":2564.0,
        "MQ7_Digital":0,
        "MQ7_Digital":1
    }
        ]
}
"""

from flask import Flask,request
from models.regressor import regress
from spacetimehandler import run_command, push_to_spaceandtime
from datetime import datetime

current_datetime = datetime.now()
formatted_datetime = current_datetime.strftime('%Y%m%d%H%M%S')

auth_code = 'java -jar sxtcli-0.0.2.jar authenticate login --privateKey="steIMQ7s1u47YGNsY+jcPzBt2XdpHpcjtnFmc/lp4KE=" --publicKey="cbZYs1wK04aI+CNm8IoM8sQtke90etogRXcy8AxxrdI=" --url="https://hackathon.spaceandtime.dev/" --userId="2468"'
biscuit_code = 'java -jar sxtcli-0.0.2.jar biscuit generate table --privateKey="62559388A05F8D65F2D7C9C36191275925900BD83EDBEED65FCCB831515ABCCC" --resources="co2_emissions_demo.emissions" --operations="CREATE,ALTER,DROP,INSERT,UPDATE,MERGE,DELETE,SELECT"'

biscuit = 'Ev8CCpQCCg5zeHQ6Y2FwYWJpbGl0eQoKZGRsX2NyZWF0ZQocY28yX2VtaXNzaW9uc19kZW1vLmVtaXNzaW9ucwoJZGRsX2FsdGVyCghkZGxfZHJvcAoKZG1sX2luc2VydAoKZG1sX3VwZGF0ZQoJZG1sX21lcmdlCgpkbWxfZGVsZXRlCgpkcWxfc2VsZWN0GAMiDwoNCIAIEgMYgQgSAxiCCCIPCg0IgAgSAxiDCBIDGIIIIg8KDQiACBIDGIQIEgMYgggiDwoNCIAIEgMYhQgSAxiCCCIPCg0IgAgSAxiGCBIDGIIIIg8KDQiACBIDGIcIEgMYgggiDwoNCIAIEgMYiAgSAxiCCCIPCg0IgAgSAxiJCBIDGIIIEiQIABIgQ9avqPWo_Gj-AtWAX9zM3qOqpcoNliPiD4sUvEQNgfcaQDITxu1dFDcxnkUHokMLZ_xvGQnQU82PpIfOGKiKbZf2OkjP-DEVQyQ5mg-U2tvyd6kx1g_7M-4Syg2GyXwOUQgiIgogY0JFjuHRuj1nKRChUYTcoz9jdq-3jl9tWVzHZ7mFTls='
authorization = 'Bearer eyJ0eXBlIjoiYWNjZXNzIiwia2lkIjoiNGE2NTUwNjYtZTMyMS00NWFjLThiZWMtZDViYzg4ZWUzYTIzIiwiYWxnIjoiRVMyNTYifQ.eyJpYXQiOjE2ODU1OTQ1MDksIm5iZiI6MTY4NTU5NDUwOSwiZXhwIjoxNjg1NTk2MDA5LCJ0eXBlIjoiYWNjZXNzIiwidXNlciI6IjI0NjgiLCJzdWJzY3JpcHRpb24iOiJkNmI2YTU1Yi1hM2QxLTRmN2MtOWMzYi00MWVlY2YzYmFhYzIiLCJzZXNzaW9uIjoiMjliNDg3ODgzN2IxOGE0NGRhZTIxZjhlIiwic3NuX2V4cCI6MTY4NTY4MDkwOTg4MSwiaXRlcmF0aW9uIjoiNTBlZWEyMGZlM2ExZjU4NDRiYjVkOGEyIn0.wult0HaI5GVf7irC6h451mEWktrSYt0VFYgDFNpPJBh8f9DRqQMFrBd-Un0Fbj90UNHLUtisdxfcOxncNJJ5Tg'

application = Flask(__name__)
@application.route('/')
def index():
    return("Welcome, please smile more")

@application.route("/predict", methods=["GET", "POST"])
def predict():
    raw_data = request.get_json(force=True)
    prediction = regress(raw_data)
    data = tuple(raw_data.values()) + (formatted_datetime, "Carbonz1-iot" , prediction, 0)
    push_to_spaceandtime(data, authorization, biscuit)
    return(prediction)

if __name__ =="__main__":
    application.run(host='0.0.0.0', port=8080)
