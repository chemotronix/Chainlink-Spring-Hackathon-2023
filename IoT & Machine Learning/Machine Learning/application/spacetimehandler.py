import subprocess
import requests

url = "https://hackathon.spaceandtime.dev/v1/sql/dml"


def run_command(command,scenario):
    process = subprocess.Popen(command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    output, error = process.communicate()

    if process.returncode == 0:
        print("Command executed successfully.")
        print("Output:", output.decode())
        if scenario == "auth":
            return(output.decode().split("\n")[1][14:-1])
        else:
            return(output.decode()[:-2])
    else:
        print("Error:", error.decode())


def push_to_spaceandtime(data, authorization, biscuit):
    payload = {"resourceId": "co2_emissions_demo.emissions","sqlText": "INSERT INTO co2_emissions_demo.emissions (Temperature,Humidity,MQ7_analog,MQ9_analog,MG811_analog,MQ135_analog,MG811_Digital,MQ7_Digital,datetime,IoT_id,co2,co) VALUES {}".format(data)}
    headers = {
        "accept": "application/json",
        "content-type": "application/json",
        "authorization":authorization,
        "biscuit": biscuit
    }

    response = requests.post(url, json=payload, headers=headers)
    print(response.text)