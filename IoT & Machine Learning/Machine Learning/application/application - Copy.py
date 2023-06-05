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

application = Flask(__name__)
@application.route('/')
def index():
    return("Welcome, please smile more")

@application.route("/predict", methods=["GET", "POST"])
def predict():
    raw_data = request.get_json(force=True)
    return (regress(raw_data))

if __name__ =="__main__":
    application.run(host='0.0.0.0', port=8080)
