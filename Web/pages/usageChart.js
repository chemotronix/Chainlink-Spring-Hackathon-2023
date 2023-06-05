import React from "react";

function usageChart() {
  const visuals = [
    "https://thingspeak.com/channels/1803223/charts/1?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&title=Temperature+%28%C2%BAC%29&type=line",

    "https://thingspeak.com/channels/1803223/charts/2?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&title=Humidity+%28%25%29&type=line",

    "https://thingspeak.com/channels/1803223/charts/3?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&title=MQ7+sensor+Data&type=line",

    "https://thingspeak.com/channels/1803223/charts/4?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&title=MQ9+Sensor+Data&type=line",

    "https://thingspeak.com/channels/1803223/charts/5?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&title=MG811+Sensor+Data&type=line",

    "https://thingspeak.com/channels/1803223/charts/6?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&title=MQ135+Sensor+Data&type=line",

    "https://thingspeak.com/channels/1803223/charts/7?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&title=MG811+Digital&type=line",

    "https://thingspeak.com/channels/1803223/charts/8?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&title=Co2+Concentration+%28PPM%29&type=line",

    "https://thingspeak.com/channels/1803223/maps/channel_show",

    "https://thingspeak.com/channels/1803223/widgets/659345"
  ];
  return (
    <div className="py-3">
      <h2 className="flex  justify-center text-[25px] mb-20">Usgae Charts</h2>
      <div className="grid xl:grid-cols-3  px-5 bg-cus lg:grid-cols-2 grid-cols-1 gap-6 mx-auto">
        {visuals.map((chart,i) => {
          return (
            <div key={i} className="rounded-lg shadow-lg  w-fit">
              <iframe
                src={chart}
                frameBorder="1"
                height={"260px"}
                width={"450px"}
                className="rounded border-solid border border-#cccccc-900"
              ></iframe>
            </div>
          );
        })}
      </div>
    </div>
  );
}

export default usageChart;
