
import pickle, json
import pandas as pd

def regress(raw_data):
    data = pd.DataFrame([raw_data])
    model=pickle.load(open("models/catboost_model.pkl","rb"))
    prediction=model.predict(data).tolist()
    prediction = round(prediction[0],4)
    return(json.dumps(prediction))