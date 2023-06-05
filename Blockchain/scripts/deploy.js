const hre = require("hardhat");

const main = async() => {
  const carbonZContractFactory = await hre.ethers.getContractFactory("CarbonZ");
  const carbonZContract = await carbonZContractFactory.deploy();
  await carbonZContract.deployed();
  console.log("Deployed awesomeness to ",carbonZContract.address);  
};

const runMain = async() => {
  try {
    await main();
    process.exit(0);
  } catch (error) {
    console.log(error);
    process.exit(1);
  }
};

runMain();

