# Open-Source Release Plan (Code + Docs Only)

## Scope
- Include: firmware code, build scripts, tools, docs, media used by docs
- Exclude for now: all KiCad/hardware design assets under `pcb/`

## Public File Manifest (v0.1)

### Include (public repo)
- `README.md`, `index.md`, `_config.yml`
- `Makefile`, `SConstruct`, `pyproject.toml`, `requirements.txt`
- `.clang-format`, `.clang-tidy`, `.gitattributes`, `.gitignore`
- `.github/workflows/request.yml`
- `board/**` (except `board/obj/**`)
- `crypto/**`
- `docs/**` (except `docs/media/turbo_ecu_clip.mov`)
- `tools/**`
- `tests/**`

### Exclude (private/internal or not ready)
- `.git/**`
- `pcb/**` (KiCad/hardware release deferred)
- `parts/**` (hardware CAD assets)
- `certs/**` (contains local key material)
- `venv/**`, `.ruff_cache/**`, `.vscode/**`, `.claude/**`, `.sconsign.dblite`
- `board/obj/**` (build artifacts)
- `AGENTS.md` (local agent instructions)
- `vapor_booster.jpg`, `vapor_dot_autos_v2.png` (unused branding assets)
- `docs/media/turbo_ecu_clip.mov` (source clip; keep compressed web assets only)

## Pre-Copy Gate
- [ ] Add `LICENSE` (MIT or Apache-2.0 recommended)
- [ ] Add `CONTRIBUTING.md`, `CODE_OF_CONDUCT.md`, `SECURITY.md`
- [ ] Confirm no secrets outside `certs/`:
  - `rg -n \"(BEGIN .*PRIVATE KEY|token|secret|password|api[_-]?key)\" -S .`
- [ ] Confirm docs/media links still work with `.mov` excluded
- [ ] Run: `make lint`
- [ ] Run: `make tidy`
- [ ] Run: `make`

## Repo Strategy
1. Create a new public repo under the shared GitHub account/org
2. Push a cleaned history-free snapshot first (fastest/safest)
3. Keep this private repo as source-of-truth until hardware files are ready
4. Optionally automate one-way sync from private -> public for selected paths

## First Public Cut Checklist
- [ ] Confirm license (MIT/BSD/Apache-2.0) and add `LICENSE`
- [ ] Add `CONTRIBUTING.md` and `CODE_OF_CONDUCT.md`
- [ ] Add `SECURITY.md` (reporting channel)
- [ ] Verify no secrets: keys, tokens, serials, private URLs, internal notes
- [ ] Verify docs reference only public assets
- [ ] Remove/omit `pcb/` from public repo
- [ ] Run quality gate: `make lint`, `make tidy`, `make test` (if applicable)
- [ ] Tag `v0.1.0` in public repo after first verified release

## Practical Migration Options

### Option A: Fresh public repo (recommended)
- Pros: clean history, lowest leak risk
- Cons: loses commit history from private repo

```bash
# from private repo root
mkdir -p /tmp/ecu-public
rsync -av --delete \
  --exclude '.git' \
  --exclude '.claude' \
  --exclude '.ruff_cache' \
  --exclude '.vscode' \
  --exclude '.sconsign.dblite' \
  --exclude 'venv' \
  --exclude 'certs' \
  --exclude 'parts' \
  --exclude 'pcb/' \
  --exclude 'board/obj' \
  --exclude 'AGENTS.md' \
  --exclude 'vapor_booster.jpg' \
  --exclude 'vapor_dot_autos_v2.png' \
  --exclude 'docs/media/turbo_ecu_clip.mov' \
  ./ /tmp/ecu-public/

cd /tmp/ecu-public
git init
git add .
git commit -m "Initial public release: code + docs"
git remote add origin git@github.com:<owner>/<public-repo>.git
git push -u origin main
```

### Option B: Keep history, filter paths
- Use `git filter-repo` to permanently remove `pcb/` from history before publishing
- Higher risk if done in-place; do this in a throwaway clone only

## Ongoing Sync (Private -> Public)
- Keep a short script for exporting selected paths into `/tmp/ecu-public`
- Re-run rsync + commit + push for each public release
- Add a release checklist issue template in the public repo

## GitHub User Switching (CLI)

### Best method: SSH host aliases per account
`~/.ssh/config`
```sshconfig
Host github-personal
  HostName github.com
  User git
  IdentityFile ~/.ssh/id_ed25519_personal
  IdentitiesOnly yes

Host github-work
  HostName github.com
  User git
  IdentityFile ~/.ssh/id_ed25519_work
  IdentitiesOnly yes
```

Use remotes like:
```bash
git remote set-url origin git@github-personal:USER/repo.git
# or
git remote set-url origin git@github-work:ORG/repo.git
```

### Commit identity per repo
```bash
git config user.name "Your Name"
git config user.email "you@users.noreply.github.com"
```

### Quick checks
```bash
git config --get user.name
git config --get user.email
ssh -T git@github-personal
ssh -T git@github-work
```

## Notes
- If you plan to open hardware later, keep placeholders in docs and add a TODO timeline
- Avoid adding generated binaries unless needed for docs/demo
