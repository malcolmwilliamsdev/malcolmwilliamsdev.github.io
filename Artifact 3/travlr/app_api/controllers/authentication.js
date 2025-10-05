const mongoose = require("mongoose");
const User = require("../models/user");
const passport = require("passport");

const register = async (req, res) => {
  // Validate the message to ensure that all parameters are present
  if (!req.body.email || !req.body.name || !req.body.password) {
    return res.status(400).json({ message: "All fields are required." });
  }

  const user = new User({
    name: req.body.name, // set user name
    email: req.body.email, // set email
    password: "", // start with an empty password
  });
  user.setPassword(req.body.password); // set the password
  const q = await user.save();

  if (!q) {
    // database returned no data
    return res.status(400).json({ message: "User registration failed." });
  } else {
    // return new user token
    const token = user.generateJWT();
    return res.status(200).json(token);
  }
};

const login = (req, res) => {
  // validate the message to ensure that all parameters are present
  if (!req.body.email || !req.body.password) {
    return res.status(400).json({ message: "All fields are required." });
  }
  // delegate authentication to passport
  passport.authenticate("local", (err, user, info) => {
    if (err) {
      // error in authentication
      return res.status(500).json({ message: "Authentication error." });
    }

    if (user) {
      // auth succeeded - generate a jwt token and return to caller
      const token = user.generateJWT();
        return res.status(200).json({ token: token });
    } else {
      // auth failed - return error message
      return res.status(401).json({ message: info.message });
    }
  })(req, res);
};

module.exports = {
  register,
  login,
};
