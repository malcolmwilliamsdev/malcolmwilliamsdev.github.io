const Users = require("../models/user");
const passport = require("passport");

const register = async (req, res) => {
  if (!req.body.email || !req.body.name || !req.body.password) {
    return res.status(400).json({ message: "All fields are required." });
  }
  try {
    const user = await Users.createUser({
      name: req.body.name,
      email: req.body.email,
      password: req.body.password,
    });
    if (!user) {
      return res.status(400).json({ message: "User registration failed." });
    } else {
      const token = Users.generateJWT(user);
      return res.status(200).json({ token });
    }
  } catch (err) {
    return res.status(400).json({ message: "User registration failed.", error: err });
  }
};

const login = (req, res) => {
  if (!req.body.email || !req.body.password) {
    return res.status(400).json({ message: "All fields are required." });
  }
  passport.authenticate("local", (err, user, info) => {
    if (err) {
      return res.status(500).json({ message: "Authentication error." });
    }
    if (user) {
      const token = Users.generateJWT(user);
      return res.status(200).json({ token });
    } else {
      return res.status(401).json({ message: info.message });
    }
  })(req, res);
};

module.exports = {
  register,
  login,
};
