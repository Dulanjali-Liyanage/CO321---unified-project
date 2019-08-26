import React, { Component } from "react";
import "./UserSignUpForm.css";
import { signUp } from "./store/actions/authActions";
import { connect } from "react-redux";
import { Redirect } from "react-router-dom";

class UserSignUpForm extends Component {
  state = {
    email: "",
    password: "",
    username: "",
    secid: ""
  };
  handleChange = e => {
    this.setState({
      [e.target.id]: e.target.value
    });
  };
  handleSubmit = e => {
    e.preventDefault();
    console.log(this.state);
    this.props.signUp(this.state);
  };
  render() {
    const { authError, auth } = this.props;

    if (!auth.uid) return <Redirect to="/signin" />;
    return (
      <div class="container-fluid">
        <div class="row no-gutter">
          <div class="d-none d-md-flex col-md-4 col-lg-6 bg-image" />
          <div class="col-md-8 col-lg-6">
            <div class="login d-flex align-items-center py-5">
              <div class="container">
                <div class="row">
                  <div class="col-md-9 col-lg-8 mx-auto">
                    <h3 class="login-heading mb-4">User Registration</h3>
                    <form onSubmit={this.handleSubmit}>
                      <div className="input-field">
                        <label htmlFor="userName">User Name</label>
                        <input
                          type="text"
                          id="username"
                          required
                          onChange={this.handleChange}
                        />
                      </div>
                      <div className="input-field">
                        <label htmlFor="email">Email</label>
                        <input
                          type="email"
                          id="email"
                          required
                          onChange={this.handleChange}
                        />
                      </div>
                      <div className="input-field">
                        <label htmlFor="secid">Security Id</label>
                        <input
                          type="text"
                          id="secid"
                          required
                          onChange={this.handleChange}
                        />
                      </div>
                      <div className="input-field">
                        <label htmlFor="password">Password</label>
                        <input
                          type="password"
                          id="password"
                          required
                          onChange={this.handleChange}
                        />
                      </div>
                      <button
                        class="btn btn-lg btn-primary btn-block btn-login text-uppercase font-weight-bold mb-2"
                        type="submit"
                      >
                        Register
                      </button>
                    </form>
                  </div>
                </div>
              </div>
            </div>
          </div>
        </div>
      </div>
    );
  }
}
const mapStateToProps = state => {
  return {
    auth: state.firebase.auth,
    authError: state.auth.authError
  };
};

const mapDispatchToProps = dispatch => {
  return {
    signUp: newUser => dispatch(signUp(newUser))
  };
};

export default connect(
  mapStateToProps,
  mapDispatchToProps
)(UserSignUpForm);
