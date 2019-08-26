import React, { Component } from "react";

import "./App.css";
import Login from "./Login";
import UserSignUpForm from "./UserSignUpForm";
import { BrowserRouter, Switch, Route } from "react-router-dom";
import Navbar from "./Navbar";
import Home from "./Home";
import View from "./View";
import Add from "./Add";
class App extends Component {
  state = {};
  render() {
    return (
      <BrowserRouter>
        <Navbar />
        <Switch>
          <Route exact path="/" component={Home} />
          <Route exact path="/signin" to component={Login} />
          <Route path="/signup-users" to component={UserSignUpForm} />
          <Route exact path="/view" to component={View} />
          <Route exact path="/add" to component={Add} />
          <Route exact path="/" to component={Add} />
        </Switch>
      </BrowserRouter>
    );
  }
}

export default App;
