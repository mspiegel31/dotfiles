# TickTick routing

Project IDs were verified with `ticktick project list` on 2026-09-08. IDs can drift; re-run `ticktick project list` at the start of every review and re-resolve by name before creating tasks.

| Vault area | TickTick project | ID |
|---|---|---|
| `Career` | 📈Career | `6a85ad862fbc5a08e3e77965` |
| `Work` | 💻Work | `6a85ad8a2fbc5a08e3e7796b` |
| `Mental Wellbeing`, `Physical Health` | 🥦Health | `6a85ad782fbc5a08e3e77959` |
| `Spirituality` | 🧘🏻‍♂Spirituality | `6a85ad7d2fbc5a08e3e7795f` |
| `Marriage` | 👩‍❤‍👨Husband | `6a85ad722fbc5a08e3e77953` |
| `Dad` | 👶Dad | `6a85adbb2fbc5a08e3e77994` |
| `Home` | 🔨Home | `6a85ad922fbc5a08e3e77971` |
| `Community & Friendships` | 🏠Community and Friendship | `6a85ad9a2fbc5a08e3e7797a` |
| `Personal Improvement` | 👱‍♂Self | `6a85aec82fbc5a08e3e779f2` |
| `Creative Practice` | 🧤Hobbies | `6a8ad19dcaa1eb7b66c16fdb` |
| `#tickler` | 🌤tickler | `6a85ad0c2fbc5a08e3e7791e` |

Standing tags: `technical-growth`, `soft-skills-growth`, `job-hunt-2026`, `road-to-40`, `effort-high`, `errands`, `kid-free-break-2026`.

## Task creation

Implementation intentions go in `--content` — the "if cue, then action" text is the part the evidence says actually drives execution, so never leave it out of the task.

```
ticktick task create \
  --title "One clear, observable action" \
  --project <projectId> \
  --tags job-hunt-2026 \
  --priority 3 \
  --content "If [specific cue/situation], then I will [small observable action]." \
  --due-date 2026-09-15T00:00:00Z
```

Priority: 0=None, 1=Low, 3=Medium, 5=High.

## Query and update

```
ticktick project list                          # refresh IDs
ticktick task search <keywords>                 # find tasks by text/tag
ticktick task filter                           # advanced filter
ticktick task get <projectId> <taskId>
ticktick task update <taskId> --id <taskId> --project <projectId> --due-date ... --content ...   # --id AND --project are both required
ticktick task complete <projectId> <taskId>
ticktick task delete <projectId> <taskId>
ticktick habit list                            # find the review habit id
ticktick habit checkin <habitId>               # weekly check-in
ticktick habit create --name "Weekly Review" --goal 1 --repeat "FREQ=WEEKLY;BYDAY=SUN"
```
