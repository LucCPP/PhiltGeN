# PhiltGeN Chess PGN Filterer
PhiltGeN is a low-dependency, easy-to-use chess PGN filtering program.

## Requirements
- A C++-compatible code editor or IDE
- A C++ compiler that can compile C++20 code
- A PGN file in standard format (see the below section)

### PGN Format
Here is an example of a PGN header compatible with PhiltGeN:
```
[Event "Rated Blitz game"]
[LichessURL "https://lichess.org/xxxxxxxx"]
[Date "2020.09.01"]
[Round "-"]
[White "white"]
[Black "black"]
[Result "1-0"]
[BlackElo "2309"]
[ECO "D52"]
[Opening "Queen's Gambit Declined"]
[TimeControl "180+0"]
[UTCDate "2020.09.01"]
[UTCTime "00:07:30"]
[Termination "Time forfeit"]
[WhiteRatingDiff "+2"]
[BlackRatingDiff "-2"]
```

Notice how quotes surround the information, and each part is surrounded by brackets with no spaces. This is an example of a header that would not work:
```
[ Event "Rated Blitz game" ]
[ LichessURL "https://lichess.org/xxxxxxxx" ]
[ Date "2020.09.01" ]
[ Round "-" ]
[ White "white" ]
[ Black "black" ]
[ Result "1-0" ]
[ BlackElo "2309" ]
[ ECO "D52" ]
[ Opening "Queen's Gambit Declined" ]
[ TimeControl "180+0" ]
[ UTCDate "2020.09.01" ]
[ UTCTime "00:07:30" ]
[ Termination "Time forfeit" ]
[ WhiteRatingDiff "+2" ]
[ BlackRatingDiff "-2" ]
```

Please ensure that your input file contains correctly-formatted games.

## Settings
This section is a table of all settings, their function, and their default value.

| Setting (for a `Game` object) | Function | Notes | Header Used |
| ----------------------------- | -------- | ----- | ----------- |
| `black(std::string)` | Sets the name black must have | Multi-word names are supported (e.g., `John Doe`) | `[Black "..."]` |
| `black_title(std::string)` | Sets the title black must have | e.g., `GM`, `FM`, `IM`, etc. | `[BlackTitle "..."]` |
| `day(int)` | Sets the day the game must have | `UTCDate` must be in YYYY.MM.DD format | `[UTCDate "YYYY.MM.DD"]` |
| `eco(std::string)` | Sets the ECO code the game must have | | `[ECO "..."]` |
| `opening(std::string)` | Sets the opening the game must have | Only the start of the opening string is compared to the argument. For example, if the argument is `French Defense`, games with the opening `French Defense` or `French Defense: Paulsen Variation` will not be discarded | `[Opening "..."]` |
| `event(std::string)` | Sets the event the game must have taken place at | | `[Event "..."]` |
| `max_black_elo(int)` | Sets the maximum ELO black can be | The argument is included in the range | `[BlackElo "..."]` |
| `max_white_elo(int)` | Sets the maximum ELO white can be | The argument is included in the range | `[WhiteElo "..."]` |
| `min_black_elo(int)` | Sets the minimum ELO black can be | The argument is included in the range | `[BlackElo "..."]` |
| `min_white_elo(int)` | Sets the minimum ELO white can be | The argument is included in the range | `[WhiteElo "..."]` |
| `month(int)` | Sets the month the game must have | `UTCDate` must be in YYYY.MM.DD format | `[UTCDate "YYYY.MM.DD"]` |
| `round(std::string)` | Sets the round the game must have | | `[Round "..."]` |
| `result(std::string)` | Sets the outcome the game must have | Standard `Result`s are `1-0`, `0-1`, `1/2-1/2`, and `*` | `[Result "..."]` |
| `site(std::string)` | Sets the site the game must have | Only the start of the site string is compared to the argument. For example, if the argument is `https://lichess.org`, games with the site `https://lichess.org` or `https://lichess.org/xxxxxxxx` will not be filtered out | `[Site "..."]` |
| `termination(std::string)` | Sets the termination the game must have | | `[Termination "..."]` |
| `time_control(std::string)` | Sets the time control the game must have | `+0` is automatically removed from `TimeControl`s and inputted time control strings. For example, `600` and `600+0` as `TimeControl`s are treated the same | `[TimeControl "..."]` |
| `white(std::string)` | Sets the name that white must have | Multi-word names are supported (e.g., `John Doe`) | `[White "..."]` |
| `white_title(std::string)` | Sets the title that white must have | e.g., `GM`, `FM`, `IM`, etc. | `[WhiteTitle "..."]` |
| `year(int)` | Sets the year the game must have | `UTCDate` must be in YYYY.MM.DD format | `[UTCDate "YYYY.MM.DD"]` |

| Setting (for a `Filter` object) | Function | Notes |
| ------------------------------- | -------- | ----- |
| `missing_info_rule(MissingInfo)` | If `MissingInfo::fail`, the game is invalid and filtered out if part of the header you activated a filter for is missing. For example, if you made a filter for results, if `Result` is missing from the PGN header, the entire game is discarded. If `MissingInfo::pass`, missing header info is skipped, but the game is not invalidated unless it does not follow your filter | The setting is set to `MissingInfo::fail` by default |

## Usage
To use PhiltGeN:
1. Download `philtgen.hpp` from `src` and drag and drop it into your workspace.
2. Add `philtgen.hpp` to the includes in the files that need access to the filter.

### Supported Operators
`&&`, `||`, and `!` are all supported to use for custom filters.

### Example Use
The following code reads an input file and writes to the output file only games where both white and black are at least rated 2400 ELO, white or black won, the opening is the French Defense or Queen's Gambit Declined, and white and black's titles are not `GM`.
```
#include "philtgen.hpp"
#include <string>

int main()
{
    // Create a game object
    Game game;

    std::string input_file{ "input.pgn" };
    std::string output_file{ "output.pgn" };

    std::string white_won{ "1-0" };
    std::string black_won{ "0-1" };

    // Conditions you want
    Filter filter = game.min_white_elo(2400)
                    && game.min_black_elo(2400)
                    && (game.result(white_won) || game.result(black_won))
                    && (game.opening("French Defense") || game.opening("Queen's Gambit Declined"))
                    && !game.white_title("GM")
                    && !game.black_title("GM");

    // Filter
    game.write(input_file, output_file, filter);

    return 0;
}
```