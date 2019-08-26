import React, { Component } from "react";
import { NavLink } from "react-router-dom";
import "./Navbar.css";

const SignOutLinks = () => {
  return (
    <div className="navbar-brand">
      <NavLink to="/signin" className="waves-effect waves-light btn-small">
        Sign In
      </NavLink>
      &nbsp;
      <NavLink to="/about" className="waves-effect waves-light btn-small">
        About Us
      </NavLink>
      &nbsp;
    </div>
  );
};

export default SignOutLinks;
