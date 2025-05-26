import React from 'react';
import NumberSpan from './NumberSpan';

interface NumberWrapperProps {
  content: string;
}

const NumberWrapper: React.FC<NumberWrapperProps> = ({ content }) => {
  // Split by numbers, keep numbers as separate tokens
  const parts = content.split(/(\d+(\.\d+)?)/g);

  return (
    <>
      {parts.map((part, i) => {
        if (/\d/.test(part)) {
          return <NumberSpan key={i}>{part}</NumberSpan>;
        }
        return <React.Fragment key={i}>{part}</React.Fragment>;
      })}
    </>
  );
};

export default NumberWrapper;
