# Nodehun-Native

## 👋 Notice - November 2023

This is a fork of [ONLYOFFICE's](https://github.com/ONLYOFFICE) fork of [Nodehun](https://github.com/ONLYOFFICE/nodehun]), that allows Nodehun to link against the actual hunspell source code. Without their great work, this wouldn't have worked. I take no credit for this, I am only maintaining the package so it will work with the latest version of Hunspell.

The primary difference between this fork and Nodehun is that the .dic and .aff files must be loaded from a file on disk, instead of via a Buffer.

Since their repo had become stale and lacked a npm package, Nodehun-Native provides feature parity with the latest Hunspell (at time of writing v1.7.2), easily installable via NPM. I've also updated the documentation and examples.

## Introduction

Nodehun aims to expose as much of hunspell's functionality as possible in an easy to understand and maintainable way, while also maintaining the performance characteristics expected of a responsible node module. 

## Features

* Native performance.
* Exposes all of hunspell's functionality:
	* Spell checking,
	* suggestions,
	* personal dictionaries and word management,
	* stems/roots of words,
	* morphological generation, and,
	* word analysis.
* TypeScript declaration file.
* Synchronous + promise-based async API.
* Extensive unit testing.
* Completely re-written using N-API (thus, stability in future v8 versions)

## Installation

	npm install nodehun-native

If you run into any build errors, make sure you satisfy the requirements for [`node-gyp`](https://github.com/nodejs/node-gyp#installation).

## Quick Start

```js
import { Nodehun } from 'nodehun'

const affix       = 'path/to/*.aff'
const dictionary  = 'path/to/*.dic'

const nodehun     = new Nodehun(affix, dictionary)

// Promise example
nodehun.suggest('colour')
		   .then(suggestions => { })

// async/await example
async function example() {
	const suggestions = await nodehun.suggest('colour')
}

// sync example
const suggestions = nodehun.suggestSync('colour')
```


## Table of Contents

1. <a href="#examples">Examples</a>
	* <a href="#checking-for-correctness">Spell checking</a>
	* <a href="#spell-suggestions">Spelling suggestions</a>
	* <a href="#add-dictionary">Adding a dictionary</a>
	* <a href="#add-word">Add a word</a>
	* <a href="#add-word-with-affix">Add a word (with affix)</a>
	* <a href="#remove-word">Remove a word</a>
	* <a href="#stem">Word stem</a>
	* <a href="#analyse">Word analysis</a>
	* <a href="#generate">Word generation</a>
2. <a href="#notes">Notes</a>
	* <a href="#improving-performance">Improving performance</a>
	* <a href="#notes-warning-on-synchronous-methods">A Warning on Synchronous Methods</a>
	* <a href="#notes-open-office-dictionaries">A Note About Open Office Dictionaries</a>
	* <a href="#notes-creating-dictionaries">A Note About Creating Dictionaries</a>
	* <a href="#notes-finding-dictionaries">Where To Get Dictionaries</a>
3. <a href="#development">Development and Contribution</a>
	* <a href="#building">Building</a>
	* <a href="#development-scripts">Scripts</a>
	* <a href="#development-notes">Notes</a>
	* <a href="#development-mentions">Mentions</a>


## <a id="examples"></a>Examples

The following section includes short examples of various exposed operations.
For complete examples, see the `/examples` directory.

### <a id="checking-for-correctness"></a>Checking for Correctness
Nodehun offers a method that returns true or false if the passed word exists in the dictionary, i.e. is "correct".

```js
await nodehun.spell('color') // => true
await nodehun.spell('colour') // => false, assuming en_US dictionary
```

### <a id="spell-suggestions"></a>Spelling Suggestions
Nodehun also offers a method that returns an array of words that could possibly match a misspelled word, ordered by most likely to be correct.

```js
await nodehun.suggest('color')
// => null (since it's correctly spelled)

await nodehun.suggest('calor')
// => ['carol','valor','color','cal or','cal-or','caloric','calorie']
```

### <a id="add-dictionary"></a>Add Dictionary
Nodehun also can add another dictionary on top of an existing dictionary object at runtime (this means it is not permanent) in order to merge two dictionaries.

```js
const en_CA = './path/to/en_CA.dic';

await nodehun.suggest('colour') // => [ ...suggestions... ]
// because "colour" is not a defined word in the US English dictionary
await nodehun.addDictionary(en_CA)
await nodehun.suggest('colour') // => null
// (since the word is considered correctly spelled now)
```

### <a id="add-word"></a>Add Word
Nodehun can also add a single word to a dictionary at runtime (this means it is not permanent) in order to have a custom runtime dictionary. If you know anything about Hunspell you can also add flags to the word.

```js
await nodehun.suggest('colour') // => [ ...suggestions...]
// because "colour" is not a defined word in the US English dictionary
await nodehun.add('colour')
await nodehun.suggest('colour') // => null
// (since 'colour' is correct now)
```

Note: _colouring_ will still be considered incorrect. See the the `addWithAffix` example below.

### <a id="add-word-with-affix"></a>Add Word (with affix)
Like the method above, except it also applies the example word's affix definition to the new word.

```js
await nodehun.suggest('colouring') // => [ ...suggestions...]
// because "colour" is not a defined word in the US English dictionary
await nodehun.addWithAffix('colour', 'color')
await nodehun.suggest('colouring') // => null
// (since 'colouring' is correct now)
```

### <a id="remove-word"></a>Remove Word
Nodehun can also remove a single word from a dictionary at runtime (this means it is not permanent) in order to have a custom runtime dictionary. If you know anything about Hunspell this method will ignore flags and just strip words that match.

```js
await nodehun.suggest('color') // => null (since the word is correctly spelled)
await nodehun.remove('color')
await nodehun.suggest('color') // => ['colon', 'dolor', ...etc ]
```

### <a id="stem"></a>Word Stems
Nodehun exposes the Hunspell `stem` function which analyzes the roots of words. Consult the Hunspell documentation for further understanding.

```js
await nodehun.stem('telling') // => [telling, tell]
```

### <a id="analyse"></a>Word Analysis
Nodehun exposes the Hunspell `analyze` function which analyzes a word and return a morphological analysis. Consult the Hunspell documentation for further understanding.

```js
await nodehun.analyze('telling') 
// with the appropriate dictionaries files, it will return:
// => [' st:telling ts:0', ' st:tell ts:0 al:told is:Vg']
```

### <a id="generate"></a>Word Generation
Nodehun exposes the Hunspell `generate` function which generates a variation of a word by matching the morphological structure of another word. Consult the Hunspell documentation for further understanding.

```js
await nodehun.generate('telling', 'ran') // => [ 'told' ]
await nodehun.generate('told', 'run') // => [ 'tell' ]
```

## <a id="notes"></a>Notes

### <a id="improving-performance"></a> Improving Performance

If the native performance isn't fast enough for your workload, you can try using an LRU cache for your operations. The idea is to cache the results of the operation and only repeat the operations on cache misses.

```js
const LRUCache = require('lru-native2')

var cache = new LRUCache({ maxElements: 1000 })

async function suggestCached() {
  let cachedResult = cache.get(word)
  if (cachedResult) {
    // cache hit
    return cachedResult
  } else {
    // cache miss
    let result = await nodehun.suggest(word)
    cache.set(word, result)
    return result
  }
}

// ... example usage:

const suggestions = await suggestCached('Wintre')
// now 'wintre' results are cached

// ... some time later...

const suggestions = await suggestCached('Wintre')
               // => this is fetched from the cache
```

Here are two LRU implementations you can consider:
* [lru-native2](https://github.com/adzerk/node-lru-native)
* [lru-cache](https://github.com/isaacs/node-lru-cache)

### <a id="notes-warning-on-synchronous-methods"></a>A Warning on Synchronous Methods
There are synchronous versions of all the methods listed above, but they are not documented as they are only present for people who really know and understand what they are doing. I highly recommend looking at the C++ source code if you are going to use these methods in a production environment as the locks involved with them can create some counterintuitive situations. For example, if you were to remove a word synchronously while many different suggestion threads were working in the background the remove word method could take seconds to complete while it waits to take control of the read-write lock. This is obviously disastrous in a situation where you would be servicing many requests.

### <a id="notes-open-office-dictionaries"></a>A Note About Open Office Dictionaries
All files must be UTF-8 to work! When you download [open office dictionaries](http://cgit.freedesktop.org/libreoffice/dictionaries/tree/) don't assume that the file is UTF-8 just because it is being served as a UTF-8 file. You may have to convert the file using the `iconv` unix utility (easy enough to do) to UTF-8 in order for the files to work.

### <a id="notes-creating-dictionaries"></a>A Note About Creating Dictionaries

If you want to create a new Hunspell dictionary you will need a base affix file. I recommend simply using one of the base affix files from the open office dictionaries for the language you are creating a dictionary for. Once you get around to creating a dictionary read the hunspell documentation to learn how to properly flag the words. However, my guess is that the vast majority of people creating dictionaries out there will be creating a dictionary of proper nouns. Proper nouns simply require the "M" flag. This is what a dictionary of proper nouns might look like:

	Aachen/M
	aardvark/SM
	Aaren/M
	Aarhus/M
	Aarika/M
	Aaron/M

Notice that the "S" flag denotes a proper noun that isn't capitalized, otherwise look in the docs.

### <a id="notes-finding-dictionaries"></a>Where To Get Dictionaries

The included dictionaries were extracted from Libre Office. The Libre Office versions have a modified aff file that makes generate() and analyze() much more useful. However, any MySpell style dictionary will work. Here are a few sources:

* [Libre Office dictionaries](http://cgit.freedesktop.org/libreoffice/dictionaries/tree/)
* [Official Aspell dictionaries](http://wordlist.aspell.net/dicts/)
* [Open Office extensions](http://extensions.services.openoffice.org/dictionary)
* [Mozilla Extensions](https://addons.mozilla.org/en-us/firefox/language-tools/)

Also, check out [@wooorm]()'s UTF-8 dictionary collection [here](https://github.com/wooorm/dictionaries).

Let the community know if you've found other dictionary repositories!

# <a id="development"></a>Development and Contribution

## <a id="building"></a>Building

To build nodehun-native, take the following steps:

```bash
# Clone the repo and enter directory
gh repo clone bjarkebech/nodehun-native
cd nodehun-native

# Install npm dependencies
npm i

# Fetch the latest changes to our submodule
git submodule update --recursive --remote

# Then, check out the latest release version of hunspell (in our case, 1.7.2).
cd src/hunspell
git checkout v1.7.2

# Go back to the root of our repository
cd ../../

# Build and run our tests!
npm run start
```

## <a id="development-scripts"></a>Scripts

The following is a a list of commands and their descriptions which may
help in development.

`npm start`: to jumpstart the development server. This will automatically recompile the
c++ source when changes are made and run the tests once more.

`npm run start-test`: if you don't want to continuously compile the c++ source, but do want
the tests to re-run when changes are made to the test files.

`npm run build`: to compile the addon once.

`npm test`: to run the tests once.

`npm run performance-test`: to run the performance tests and output updated graphs. (see `test/performance`)

## <a id="development-notes"></a>Notes

Make `node-gyp` build faster by increasing the number of cores it uses:

```bash
export JOBS=max
npm run build # super fast now!
```

## <a id="development-mentions"></a>Mentions

Special thanks to [@nathanjsweet](https://github.com/nathanjsweet) for his grass roots efforts with this project, including the `hunspell-distributed` package upon which this library relies to provide buffer-based Hunspell initialization.

Huge props to [@Wulf](https://github.com/Wulf) and [@ONLYOFFICE](https://github.com/ONLYOFFICE) for their great work on Nodehun and the fork.