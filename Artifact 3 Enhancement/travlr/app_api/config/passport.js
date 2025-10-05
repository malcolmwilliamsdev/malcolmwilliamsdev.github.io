const passport = require("passport");
const LocalStrategy = require("passport-local").Strategy;
const Users = require("../models/user");

passport.use(
  new LocalStrategy(
    {
      usernameField: "email",
    },
    async (username, password, done) => {
      try {
        const user = await Users.findUserByEmail(username);
        if (!user) {
          return done(null, false, { message: "Incorrect username." });
        }
        if (!Users.validatePassword(user, password)) {
          return done(null, false, { message: "Incorrect password." });
        }
        return done(null, user);
      } catch (err) {
        return done(err);
      }
    }
  )
);
