import Head from "next/head";
import Image from "next/image";
import { useRef,useState } from "react";
import ContactSection from "../components/ContactSection";
import Hero from "../components/Hero";
import QuoteSection from "../components/QuoteSection";
import ThirdSection from "../components/ThirdSection";
import DefaultLayout from "../layouts/DefaultLayout";
import { AnimatePresence, AnimateSharedLayout, motion } from "framer-motion";
import styles from "../styles/Home.module.css";
import Loader from "../components/Loader";

export default function Home() {
  const aboutRef = useRef();
  const [loading, setLoading] = useState(false);

  const refScroll = (e) => {
    e?.current?.scrollIntoView();
  };
  return (
    <div>
      <Head>
        <title>CarbonZ</title>
        <meta name="description" content="Generated by create next app" />
        <link rel="icon" href="/favicon.ico" />
      </Head>

      <AnimateSharedLayout type="crossfade">
          <AnimatePresence>
            {loading ? (
              <motion.div className="cus-scroll overflow-y-hidden" key="loader">
                <Loader setLoading={setLoading} />
              </motion.div>
            ) : (
      <DefaultLayout>
        
      <main
        className="overflow-x-hidden bg-transparent text-black"
        data-theme="emerald"
      >
          <Hero />
          <QuoteSection />
          <ThirdSection aboutRef={aboutRef} />
          <ContactSection />
      </main>
      </DefaultLayout>)}
       </AnimatePresence>
       </AnimateSharedLayout>
    </div>
    
  );
}
