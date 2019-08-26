import React, { Component } from "react";
import { NavLink } from "react-router-dom";
import "./Navbar.css";
import { connect } from "react-redux";
import { signOut } from "./store/actions/authActions";

const SignInLinks = props => {
  return (
    <div className="navbar-brand">
      <NavLink
        to="/view-profile"
        className="btn btn-outline-success my-2 my-sm-0 "
      >
        {props.profile.username}
      </NavLink>
      &nbsp;
      <NavLink to="/add" className="waves-effect waves-light btn-small">
        Add
      </NavLink>
      &nbsp;
      <NavLink to="/view" className="waves-effect waves-light btn-small">
        View
      </NavLink>
      &nbsp;
      <NavLink
        to="/notifications"
        className="waves-effect waves-light btn-small"
      >
        Notifications
      </NavLink>
      &nbsp;
      <NavLink to="/change" className="waves-effect waves-light btn-small">
        Change
      </NavLink>
      &nbsp;
      <a class="waves-effect waves-light btn-small" onClick={props.signOut}>
        Log Out
      </a>
      &nbsp;
    </div>
  );
};

const mapDispatchToProps = dispatch => {
  return {
    signOut: () => dispatch(signOut())
  };
};

export default connect(
  null,
  mapDispatchToProps
)(SignInLinks);
