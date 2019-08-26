import React, { Component } from "react";
import "./UserSignUpForm.css";
import { connect } from "react-redux";
import { signIn } from "./store/actions/authActions";
import { Redirect } from "react-router-dom";

class Login extends Component {
  state = {
    email: "",
    password: ""
  };
  handleChange = e => {
    this.setState({
      [e.target.id]: e.target.value
    });
  };
  handleSubmit = e => {
    e.preventDefault();

    this.props.signIn(this.state);
  };
  render() {
    const { authError, auth } = this.props;
    if (auth.uid) return <Redirect to="/" />;
    return (
      <div class="container-fluid">
        <div class="row no-gutter">
          <div class="d-none d-md-flex col-md-4 col-lg-6 bg-image" />
          <div class="col-md-8 col-lg-6">
            <div class="login d-flex align-items-center py-5">
              <div class="container">
                <div class="row">
                  <div class="col-md-9 col-lg-8 mx-auto">
                    <h3 class="login-heading mb-4">User SignIn</h3>
                    <form onSubmit={this.handleSubmit}>
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
                        Sign In
                      </button>
                      <div className="red-text center">
                        {authError ? <p>{authError}</p> : null}
                      </div>
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
    authError: state.auth.authError,
    auth: state.firebase.auth
  };
};

const mapDispatchToProps = dispatch => {
  return {
    signIn: creds => dispatch(signIn(creds))
  };
};

export default connect(
  mapStateToProps,
  mapDispatchToProps
)(Login);
