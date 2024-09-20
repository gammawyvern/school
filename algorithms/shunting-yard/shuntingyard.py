"""
This can take simple infix math equations (as strings),
and convert them to reverse polish notation based on the shunting yard algorithm. (designed by Dijkstra, of course).
It uses 2 stacks to pull apart and reassemble the equation.
"""

########################################
# This is a very simple stack data
# struct for holding equations
########################################

class Stack(object):
  stack: list;
  size: int;

  def __init__(self):
    self.stack = [];
    self.size = 0;

  def push(self, obj: object):
    self.stack.append(obj);
    self.size += 1;
  
  def pop(self):
    if (self.size):
      self.size -= 1;
      return self.stack.pop(self.size);
    else:
      return None;

  def peek(self):
    if (self.size):
      return self.stack[self.size-1];
    else:
      return None;

  def __str__(self):
    out = "";
    for obj in self.stack:
      out += f"{obj} ";
    return out;

########################################
# This section is setup for the logic
# for moving operators from the operator
# stack into the output stack in the
# correct order.
########################################

# This is for order of operations, which we use to ensure that all
# operations are pushed in order to the output stack
prio = {
    '^':4,
    '*':3, '/':3,
    '+':2, '-':2
};

# Exponents is a right associated operation
# For simplcity, true = left association 
# and false = right assoctiation
associativity = {
    '^':False,
    '*':True, '/':True,
    '+':True, '-':True
}

########################################
# This is the actual shunting yard 
# algorithm function. It assumes the
# passed string has a space between all
# characters, and that parenthesis are
# correct. It also treats any characters
# as variables
########################################

def shunting(equation: str) -> str:
  equation = equation.split();

  # The output stack represents the final RPN equation
  output = Stack();
  # The operator stack is used to put all operators in the correct
  # order before being pushed to output stack
  operators = Stack();

  # Goes through each part of equation
  for input in equation:
    # Numbers/variables get pushed to the output right away
    if (input.isnumeric() or input.isalpha()):
      output.push(input);

    # Left parenthesis always gets pushed to operator stack,
    # and is used when a right parenthesis is reached
    elif (input == '('):
      operators.push(input);

    # Right parenthesis adds all operators between it and its
    # matching left parenthesis
    elif (input == ')'):
      while (operators.peek() != '('):
          output.push(operators.pop());
      # Remove left parenthesis
      operators.pop();

    # Case for any 'valid' operator (valid here means the operator is in the prio and associativity dictionaries above) 
    elif (list(prio.keys()).count(input)):
      # First, we want to make sure there are operators on the operator stack and
      #   we don't want to go outside of current left parenthesis until we hit right parenthesis
      while (operators.peek() and operators.peek() != '(' and 
              # Next, we only want to add operators from operators stack to output stack while the input prio is less
              (prio[input] < prio[operators.peek()] or 
                # Next, we want to add operators from operators stack to output stack while the input prio is the same if it has left associativity
                (prio[input] == prio[operators.peek()] and associativity[input]))):
        output.push(operators.pop());

      # Push the new operator once other checks and moves are done
      operators.push(input);

    # Anything else is ignored because it isn't valid

  # At the end we have to add the rest of the operator stack to the output stack (if anything is left)
  while (operators.peek()):
    output.push(operators.pop());

  return str(output);

########################################
# This is testing / running the program
########################################

if (__name__ == "__main__"):
    testEquation = "4 * 2 * ( 4 - 2 ) ^ 3 + 3";
    print(f"Infix notation:\n\t {testEquation}\n");
    testOutput = shunting(testEquation);
    print(f"Postfix/Reverse Polish notation:\n\t {testOutput}\n");

