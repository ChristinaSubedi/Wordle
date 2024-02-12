Wordle is a game similar to Mastermind. The goal is to guess a word in as few attempts as possible. For each guess, feedback is provided on which characters are correct and at the correct position (marked in green), which characters occur in the final word but at another position (marked in yellow), and which characters do not occur in the final word (marked in gray).
In the example above, the ’G’ in the first row is marked yellow as it occurs in PROG but at the last position and not the third. The second ’O’ in the second row is marked green as it occurs in the same position in COOL and in PROG. Notice that the first ’O’ is marked gray. See below for these special cases.
Further examples and a playable version can be found online:

                    https://www.nytimes.com/games/wordle/index.html

                    
Game Principle
The game is determined by the length of the words one plays with and a dictionary containing all possible words of this length. In the usual setting, one plays with a subset of English words where all words are five characters long.

The course of the game is described by

1. Chose a random word from the dictionary
2. Retrieve a valid guess from the player
3. Provide feedback on which characters are CORRECT / WRONGPOS / WRONG 4. Repeat until all characters are correct

The random word should be chosen uniformly at random from the whole dictionary. Every word should have the same probability of being chosen.
A guess is valid when it is in lower case and is one of the words in the dictionary provided. Therefore, one has to keep track of all possible words and efficiently check whether a word is in the dictionary.


Feedback

After the player has guessed, they are provided feedback on their guess. There, a character at position 𝑖 is marked green when the character in the secret word at position 𝑖 coincides with the guess at position 𝑖.

Otherwise, the character at position 𝑖 in guess is marked yellow when the character appears in the secret word at a different position 𝑗 than in the guess. Additionally, the character in position 𝑗 in the secret word must not already be used to indicate another position in the guess green or yellow.

If neither case applies, the character at position 𝑖 in guess is not marked green nor yellow, it is marked gray.

Quantum Wordle

We now want to get a bit more advanced and introduce concepts of quantum physics into wordle. There no longer is a single secret word but instead a quantum state of two dual words that represents the secret. Both words have the same length and share no letters. That is, the sets of their characters are disjoint.

The user has to guess either of the two words to win.

If the matched characters are all from the same word, the resulting feedback is identical to normal wordle. The same rules as in normal wordle apply.
If there are matched characters in both words, the feedback uses new quantum states. When a character is in the same position in the guess as in one of the secret words, a quantum correct feedback is returned. When a character is in one of the secret words but not in the same position as in the guess, a quantum wrong position feedback is returned. When a character is in neither of the secret words, a wrong feedback is returned.

User Interface

You can either play on the command line or using a graphical user interface. Build the project using the command make. This will create the executable bin/wordle_debug and bin/wordle_opt. 

Run the game using the command

bin/wordle_debug [k] [dictionary] [quantum].

To run normal wordle execute 

bin/wordle_debug 5 data/5.txt.
