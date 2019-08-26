import React, { Component } from "react";
import { Link } from "react-router-dom";
import SignInLinks from "./SignInLinks";
import SignOutLinks from "./SignOutLinks";

import { connect } from "react-redux";
import AdminLinks from "./AdminLinks";

const Navbar = props => {
  const { auth, profile } = props;
  console.log(auth);
  var links = auth.uid ? <SignInLinks profile={profile} /> : <SignOutLinks />;
  if (profile.username == "Admin") {
    links = <AdminLinks profile={profile} />;
  }
  return (
    <nav class="navbar navbar-expand-lg navbar-light bg-dark">
      <Link to="/" class="navbar-brand text-white" href="#">
        Security Dept
      </Link>
      <div>{links}</div>
    </nav>
  );
};

const mapStateToProps = state => {
  return {
    auth: state.firebase.auth,
    profile: state.firebase.profile
  };
};

export default connect(mapStateToProps)(Navbar);
