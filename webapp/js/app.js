import React from "react";
import ReactDOM from "react-dom";


class Toggle extends React.Component {
  constructor() {
    super();
    this.pins = {};
    // This binding is necessary to make `this` work in the callback
    this.togglePin = this.togglePin.bind(this);
  }

togglePin(pin) {
  let value = this.pins[pin] === 0 ? 1 : 0;
  fetch('/io?pin=' + pin + '&value=' + value, {
    method: 'POST',
    headers: {
      'Accept': 'application/json',
      'Content-Type': 'application/json',
    }
  }).then( () => {
    this.pins[pin] = value;
  });
}

render() {
  return (
    <div>
      <div>toggle react</div>
      <button onClick={() => this.togglePin(0)}>
        {this.pins[0] === 1 ? 'Pin0 ON' : 'Pin0 OFF'}
      </button>
    </div>
  );
}
  
}

ReactDOM.render(
  <Toggle />,
  document.getElementById('app')
);