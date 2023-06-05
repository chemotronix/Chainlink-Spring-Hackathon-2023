require("@nomicfoundation/hardhat-toolbox");
require("dotenv").config();

/** @type import('hardhat/config').HardhatUserConfig */
module.exports = {
  solidity: "0.8.17",

  networks: {
    hardhat: {

    },

    mumbai: {
      url: process.env.quicknode_rpc,
      accounts: [`0x${process.env.private_key}`],
      gas: 2100000,
      gasPrice: 8000000000,
    },
  },
};
