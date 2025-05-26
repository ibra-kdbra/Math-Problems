import React from 'react';
import '../styles/numberspan.css';

const NumberSpan: React.FC<{children: React.ReactNode}> = ({ children }) => (
  <span className="number-span">
    {children}
  </span>
);

export default NumberSpan;
