# Conventions 

This file describe the general conventions used on the project.

## Commit Message Formats

### Types

The next types construct the beginning of the commit message:

* `[FEAT]`: commits that add or remove a new feature.

* `[FIX]`: commits that fix a bug.

* `[REFACTOR]`: commits that rewrite/restructure the code.

* `[PERFORMANCE]`: commits that improve performance.

* `[STYLE]`: commits that change the style.

* `[TEST]`: commits that affect tests.

* `[DOCS]`: commits that affect documentation.

* `[BUILD]`: commits that affect build components.

* `[CHORE]`: commits that do not fit the previous types.

* `[MIX]`: commits that are a mix of the previous types.

### Description

After the type, the commit message contains a concise description of the change:

* Is a **mandatory** part of the format.

* Add one space between the type and the description.

* Capitalize the first letter.

* Use the imperative, present tense.

* Add a dot (`.`) at the end.

### Issue Idenfitication

If the commit is associated to an issue, you must then add the issue number after the description :

* Add one space between the description and the issue number.

* Add the issue with the following format `(XXXX)`.
