import Head from "next/head";
import Navbar from "../components/Navbar";
import style from "../styles/signedIn.module.css";
import * as RiIcons from "react-icons/ri";
import { useState } from "react";
import Footer from "../components/Footer";
import Link from "next/link";
import { ethers } from "ethers";
import connectContract from "../utils/connectContract";
import { toast } from "react-toastify";

const BuyToken = () => {
  const [showBuying, setShowBuying] = useState(false);
  const [buying, setBuying] = useState("Co2E");
  const [transferAmount, setTransferAmount] = useState("");
  const [isLoading, setIsLoading] = useState(false);

  const toggleBuying = () => {
    setShowBuying(!showBuying);
  };
  const changeBuying = (e) => {
    setBuying(e);
    setShowBuying(!showBuying);
  };
  const createEvent = async (cid) => {
    try {
      const chemContract = connectContract();

      if (chemContract) {
        let eventDataCID = cid;

        const txn = await chemContract.buyCredit(eventDataCID, transferAmount, {
          gasLimit: 900000,
        });
        console.log("Minting...", txn.hash);
        console.log("Minted -- ", txn.hash);
        toast.success("Token Purchased");
      } else {
        console.log("Error getting contract.");
        toast.error("Error getting contract.");
      }
    } catch (error) {
      console.log(error, "err");
      toast.success("Token Purchase failed");
    }
  };
  async function handleSubmit(e) {
    e.preventDefault();

    const body = {
      amnt: transferAmount,
    };

    try {
      const response = await fetch("/api/store-event-data", {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify(body),
      });
      if (response.status !== 200) {
        alert("Oops! Something went wrong. Please refresh and try again.");
      } else {
        console.log("Form successfully submitted!");
        let responseJSON = await response.json();
        await createEvent(responseJSON.cid);
      }
      // check response, if success is false, dont take them to success page
    } catch (error) {
      alert(
        `Oops! Something went wrong. Please refresh and try again. Error ${error}`
      );
    }

    setShowBuying(!showBuying);
  }

  return (
    <div>
      <Head>
        <title>Chemotronix</title>
        <meta name="description" content="Generated by create next app" />
        <link rel="icon" href="/favicon.ico" />
      </Head>

      <div className={style.bg}></div>
      <main className="flex justify-center ">
        <div className="container px-5">
          <Navbar />

          <div className="mt-24 w-full flex justify-start">
            <Link href={"/signedIn"}>
              <div className="bg-green-800 h-16 rounded-md cursor-pointer px-12 flex items-center">
                <p className=" text-white">Back</p>
              </div>
            </Link>
          </div>

          <div className="mt-16 w-full flex justify-center">
            <div className="flex flex-col items-center w-full md:w-1/2">
              <div className="flex flex-col justify-center mb-7">
                <p>buy carbon credit</p>
                <h1 className="text-4xl md:text-6xl font-bold">
                  How would you like to buy
                </h1>
              </div>
              <form
                onSubmit={handleSubmit}
                className="w-full flex flex-col items-center"
              >
                <div className="w-full relative">
                  <input
                    type="number"
                    id="transferAmount"
                    onChange={(e) => setTransferAmount(e.target.value)}
                    value={transferAmount}
                    placeholder="Enter amount"
                    className="bg-green-100 pl-36 w-full border-2 border-green-300 cursor-pointer rounded-lg px-8 py-6 flex  justify-center items-center"
                  />
                  <div
                    className="absolute top-3 left-10 flex  items-center cursor-pointer"
                    onClick={toggleBuying}
                  >
                    <p className="font-bold">{buying}</p>
                    <RiIcons.RiArrowDropDownLine className="text-6xl" />
                  </div>

                  {showBuying && (
                    <div className="absolute top-[90px] w-44 z-10">
                      <div className="bg-slate-50 w-full border-2 cursor-pointer py-6 border-slate-50 border-t-green-800">
                        <ul className="flex flex-col w-full justify-center items-center">
                          <li
                            className=" py-3 hover:bg-slate-300 w-full text-center"
                            onClick={() => changeBuying("Co2E")}
                          >
                            <p>Co2E</p>
                          </li>
                          <li
                            className=" py-3 hover:bg-slate-300 w-full text-center"
                            onClick={() => changeBuying("USDT")}
                          >
                            <p>USDT</p>
                          </li>
                        </ul>
                      </div>
                    </div>
                  )}
                </div>
                <button
                  type="submit"
                  disabled={isLoading}
                  className=" text-white bg-green-800 flex justify-center w-4/6 h-16 mt-10 rounded-md cursor-pointer px-12 flex items-center"
                >
                  {isLoading ? "Purchasing" : "Proceed with purchase"}
                </button>
              </form>
            </div>
          </div>
        </div>
      </main>

      
    </div>
  );
};

export default BuyToken;
