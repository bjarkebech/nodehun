const path = require('path')

module.exports = {
  en_US: {
    affix: path.resolve(__dirname, "./en_US.aff"),
    dictionary: path.resolve(__dirname, "./en_US.dic"),
  },
  en_CA: {
    affix: path.resolve(__dirname, "./en_CA.aff"),
    dictionary: path.resolve(__dirname, "./en_CA.dic"),
  },
  da_DK: {
    affix: path.resolve(__dirname, "./da_DK.aff"),
    dictionary: path.resolve(__dirname, "./da_DK.dic"),
  },
};